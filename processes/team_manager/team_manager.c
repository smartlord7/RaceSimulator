/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include "unistd.h"
#include "signal.h"
#include "stdlib.h"
#include "string.h"
#include "pthread.h"
#include "../../structs/malfunction/malfunction_t.h"
#include "../../helpers/log_generator/log_generator.h"
#include "../../util/process_manager/process_manager.h"
#include "../../ipcs/message_queue/msg_queue.h"
#include "../../util/numbers/numbers.h"
#include "../../util/file/file.h"
#include "team_manager.h"

// endregion dependencies

// region private functions prototypes

/**
 * @def race_car_worker
 * @brief Worker function executed when the race car thread is created.
 *
 * @param race_car
 * The race car information.
 *
 */
static void manage_box(race_box_t *box);

/**
 * @def race_car_worker
 * @brief Function responsible for starting the car behavior simulation.
 *
 * @param data
 * Data about the car.
 *
 */
static void * race_car_worker(void *data);

/**
 * @def simulate_car
 * @brief Function that simulates the behavior of a race car.
 *
 * @param car
 * Race car to simulate.
 *
 */
static void simulate_car(race_car_t *car);

/**
 * @def wait_race_end
 * @brief Function that allows the calling process to wait for the race to end in order to avoid zombie processes.
 *
 */
static void wait_race_end();

// endregion private functions prototypes

// region global variables.

int unn_pipe_fds[2];

// endregion global variables.

// region public functions

void team_manager(void *data) {
    race_team_t *team = (race_team_t *) data;
    DEBUG_MSG(PROCESS_RUN, ENTRY, team->team_name, getpid())

    // ignore all signals.
    signal(SIGSEGV, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGUSR1, SIG_IGN);

    // init all team ipcs and variables.
    init_mutex(&team->team_box.mutex, true);
    init_mutex(&team->team_box.available, true);
    init_mutex(&team->pipe_mutex, true);
    init_cond(&team->team_box.cond, true);
    close_fd(unn_pipe_fds[0]);
    team->team_box.team = team;
    team->team_box.car_dispatched = true;
    team->num_cars_safety = 0;

    pthread_t car_threads[MAX_MAX_CARS_PER_TEAM];
    int i;

    // wait for the race to start.
    if (!wait_race_start()) {
        // if the user sends a SIGINT before the race starts.
        close_fd(unn_pipe_fds[1]);
        DEBUG_MSG(PROCESS_EXIT, ENTRY, team->team_name)

        return;
    }

    i = 0;

    // create team car threads.
    while (i < team->num_cars) {
        create_thread(shm->race_cars[team->team_id][i].name, &car_threads[i], race_car_worker, &shm->race_cars[team->team_id][i]);
        i++;
    }

    DEBUG_MSG(FUNCTION_RUN, ENTRY, TEAM_BOX)

    manage_box(&team->team_box);

    DEBUG_MSG(FUNCTION_EXIT, ENTRY, TEAM_BOX)

    wait_threads(team->num_cars, car_threads);

    wait_race_end();
    close_fd(unn_pipe_fds[1]);

    DEBUG_MSG(PROCESS_EXIT, ENTRY, team->team_name, getpid())
}
// endregion public functions

// region private functions

void wait_race_end() {
    SYNC
    while (shm->state != FINISHED) {
        wait_cond(&shm->cond, &shm->mutex);
    }
    END_SYNC
}

void manage_box(race_box_t *box) {
    race_car_t *car = NULL;
    race_team_t *team = NULL;
    int repair_time;

    team = box->team;
    box->current_car = NULL;
    box->car_dispatched = false;

    while (true) {

        box->state = FREE;
        generate_log_entry(BOX_STATE_CHANGE, box, NULL);

        // wait for an incoming car or for the number of cars in SAFETY mode to be different of 0.
        // If the race is interrupted/finished then it should leave the loop.
        SYNC_BOX_COND
        while (shm->state == RUNNING && team->num_cars_safety == 0 && box->current_car == NULL) {
            wait_cond(&box->cond, &box->mutex);
        }
        END_SYNC_BOX_COND

        // check if the race is still running.
        if (shm->state != RUNNING) {
            if (box->current_car != NULL) {
                SYNC_CAR_COND
                box->car_dispatched = true;
                notify_cond(&car->cond);
                END_SYNC_CAR_COND
            }

            END_SYNC_BOX_COND
            return;
        }

        // there is at least one car in SAFETY mode.
        if (team->num_cars_safety > 0) {

            box->state = RESERVED;
            generate_log_entry(BOX_STATE_CHANGE, box, NULL);

            // stuck till there are no more cars in SAFETY mode.
            while (true) {

                // wait for the reception of a car in safety mode.
                SYNC_BOX_COND
                while (shm->state == RUNNING && team->num_cars_safety > 0 && box->current_car == NULL) {
                    wait_cond(&box->cond, &box->mutex);
                }
                END_SYNC_BOX_COND

                // if there are no more cars in SAFETY mode then it breaks the loop.
                if (team->num_cars_safety == 0) {
                    END_SYNC_BOX_COND
                    break;
                }

                // check if the race is still active.
                if (shm->state != RUNNING) {

                    // dispatch the car in the box since the race has been interrupted
                    if (shm->state == INTERRUPTED && box->current_car != NULL) {
                        SYNC_CAR_COND
                        box->car_dispatched = true;
                        notify_cond(&car->cond);
                        END_SYNC_CAR_COND
                    }

                    return;
                }

                // a new car is loaded into the box.
                car = box->current_car;

                // check if the arriving car is malfunctioning or it just needs refuel.
                if (car->malfunctioning) {

                    generate_log_entry(BOX_MALFUNCTION_ENTER, box, NULL);

                    repair_time = random_int(config.min_repair_time, config.max_repair_time);

                    generate_log_entry(BOX_REPAIR, box, NULL);
                    sync_sleep(repair_time);

                    // notify the car that it is dispatched due to an earlier race interruption.
                    if (shm->state == INTERRUPTED) {

                        SYNC_CAR_COND
                        box->car_dispatched = true;
                        notify_cond(&car->cond);
                        END_SYNC_CAR_COND

                        return;
                    }
                } else {
                    generate_log_entry(BOX_REFUEL_ENTER, box, NULL);
                }

                generate_log_entry(BOX_REFUEL, box, NULL);
                sync_sleep(REFUEL_TIME);

                car->remaining_fuel = config.fuel_tank_capacity;

                //notify the car that has been dispatched.
                SYNC_CAR_COND
                box->car_dispatched = true;
                notify_cond(&car->cond);
                END_SYNC_CAR_COND
                generate_log_entry(BOX_LEAVE, box, NULL);

                box->current_car = NULL;
            }
        } else if (box->current_car != NULL) {

            // receive the first car in non SAFETY mode that enters the box.
            car = box->current_car;

            box->state = OCCUPIED;
            generate_log_entry(BOX_STATE_CHANGE, box, NULL);

            generate_log_entry(BOX_REFUEL, box, NULL);
            sync_sleep(REFUEL_TIME);

            car->remaining_fuel = config.fuel_tank_capacity;

            //notify the car that has been dispatched
            SYNC_CAR_COND
            box->car_dispatched = true;
            notify_cond(&car->cond);
            END_SYNC_CAR_COND
            generate_log_entry(BOX_LEAVE, box, NULL);

            box->current_car = NULL;
        }
    }
}

void * race_car_worker(void *data) {
    race_car_t * car = (race_car_t *) data;
    DEBUG_MSG(THREAD_RUN, ENTRY, car->name)

    simulate_car(car);

    DEBUG_MSG(THREAD_EXIT, ENTRY, car->name)

    pthread_exit(EXIT_SUCCESS);
}

void simulate_car(race_car_t * car) {
    int box_needed;
    float fuel_per_lap, min_fuel1, min_fuel2;
    malfunction_t malfunction = {0};
    race_car_state_change_t car_state_change = {0};
    race_box_t * box = NULL;
    race_team_t * team = NULL;

    box_needed = false;
    fuel_per_lap = config.lap_distance / car->speed * car->consumption;
    min_fuel1 = REFUEL_MIN_LAPS1 * fuel_per_lap;
    min_fuel2 = REFUEL_MIN_LAPS2 * fuel_per_lap;
    box = &car->team->team_box;
    team = car->team;
    car_state_change.car_team_index = car->team_index;

    // the car is ready to race.
    CHANGE_CAR_STATE(RACE);

    // the car simulation itself.
    while (true) {
        // try to gain access to the box, if needed.
        // the car needs to access the box if the following conditions, in the presented short circuit order, are satisfied:
        // - it needs to be refueled or fixed.
        // - it is not about to complete the last lap (why refueling / fixing if its about to cross the finish line?).
        // - it is one step or less closer of the position = 0m (where the box is located).
        // - if the box is reserved and the car is in safety mode or the box is free.
        // - if the car can gain access to the box's lock.
        if (shm->state == RUNNING && box_needed && car->completed_laps != config.laps_per_race - 1 && car_close_to_box &&
            ((box->state == RESERVED && car->state == SAFETY) || box->state == FREE) && pthread_mutex_trylock(&box->available) == 0) {

            // spend the needed fuel to reach the box. If the car's position = 0 then the spent fuel is also 0.
            if (car->current_pos > 0) {
                car->remaining_fuel -= (((config.lap_distance - car->current_pos) / car->current_speed) *
                                        car->current_consumption);
            }

            // if the car has reached an illegal level of fuel (aka less than 0) to reach the box then it means actually it had no fuel to reach the box.
            if (car->remaining_fuel < 0) {

                // the car's journey has ended :(
                CHANGE_CAR_STATE(DISQUALIFIED);
                generate_log_entry(CAR_OUT_OF_FUEL, car, NULL);

                // unlock the previously locked box lock.
                unlock_mutex(&box->available);

                exit_thread();
            }


            // the car reached the box and now changed its state to IN_BOX.
            CHANGE_CAR_STATE(IN_BOX);

            SYNC
            // reset the car's position to 0.
            car->current_pos = 0;
            car->num_box_stops++;
            END_SYNC

            // notify the box that a new car has arrived.
            SYNC_BOX_COND
            box->current_car = car;
            notify_cond(&box->cond);
            END_SYNC_BOX_COND

            // wait for the box notification that the work on the car is done.
            SYNC_CAR_COND
            while (box->car_dispatched == false) {
                wait_cond(&car->cond, &car->cond_mutex);
            }
            END_SYNC_CAR_COND

            unlock_mutex(&box->available);

            if (shm->state == INTERRUPTED) {
                CHANGE_CAR_STATE(FINISH);
                generate_log_entry(CAR_FINISH, car, NULL);
                exit_thread();
            }

            // the car is no longer malfunctioning.
            car->malfunctioning = false;

            car->num_refuels++;

            // the car is now ready to race again.
            CHANGE_CAR_STATE(RACE);

            SYNC
            car->completed_laps++;
            END_SYNC

            generate_log_entry(CAR_COMPLETE_LAP, car, NULL);

            // the car needs the box no more.
            box_needed = false;
        }

        // check if the car has some malfunction.
        if (rcv_msg(malfunction_q_id, (void *) &malfunction, sizeof(malfunction), car->car_id) > 0) {
            DEBUG_MSG(malfunction.description, EVENT, "");

            // if a malfunction is detected, the car's state changes to SAFETY.
            car_state_change.malfunctioning = true;
            car->malfunctioning = true;
            CHANGE_CAR_STATE(SAFETY);
            car_state_change.malfunctioning = false;

            car->num_malfunctions++;

            // the car needs to access the box.
            box_needed = true;
        }

        // check if the car is about to cross pos = 0.
        if (car->current_pos + car->current_speed >= config.lap_distance) {

            // consume the needed fuel to reach the finish line. If the car is in position 0 then the spent fuel is also 0.
            if (car->current_pos != 0) {
                car->remaining_fuel -= (((config.lap_distance - car->current_pos) / car->current_speed) *
                                        car->current_consumption);
            }

            // if the car has reached an illegal level of fuel (aka less than 0) to reach pos = 0 then it means actually it had no sufficient fuel to reach pos = 0.
            if (car->remaining_fuel <= 0) {

                // the car's journey has ended :(
                CHANGE_CAR_STATE(DISQUALIFIED);
                generate_log_entry(CAR_OUT_OF_FUEL, car, NULL);

                exit_thread();
            }

            SYNC
            // increase the number of completed laps.
            car->completed_laps++;
            // reset the car's position to 0.
            car->current_pos = 0;
            END_SYNC

            generate_log_entry(CAR_COMPLETE_LAP, car, NULL);

            if (shm->state == INTERRUPTED) {
                CHANGE_CAR_STATE(FINISH);
                generate_log_entry(CAR_FINISH, car, NULL);

                exit_thread();
            }

            // check if the car has completed all the race laps.
            if (car->completed_laps == config.laps_per_race) {

                // change the car's state to FINISHED since it has reached the race's end.
                CHANGE_CAR_STATE(FINISH);
                generate_log_entry(CAR_FINISH, car, NULL);

                exit_thread();
            }

            continue;
        }

        // spend the needed fuel.
        car->remaining_fuel -= car->current_consumption;
        // do a discrete spatial step :-).
        SYNC
        car->current_pos += car->current_speed;
        END_SYNC

        DEBUG_MSG(CAR_MOVE, EVENT, car->name, car->current_pos)

        // check if the car's fuel as reached an end.
        if (car->remaining_fuel <= 0) {

            // the car's journey has ended :(
            CHANGE_CAR_STATE(DISQUALIFIED);
            generate_log_entry(CAR_OUT_OF_FUEL, car, NULL);

            exit_thread();
        }

        // check if the car has fuel to at least run for 4 more laps.
        if (!box_needed && car->remaining_fuel <= min_fuel1) {
            generate_log_entry(CAR_MIN_FUEL1_REACHED, car, NULL);

            // the car needs to be refueled since it has insufficient fuel for more 4 laps.
            box_needed = true;
        }

        // check if the car has reached a critical level of fuel.
        if (car->state != SAFETY && car->remaining_fuel <= min_fuel2) {

            generate_log_entry(CAR_MIN_FUEL2_REACHED, car, NULL);

            // the car must assume SAFETY mode.
            CHANGE_CAR_STATE(SAFETY);
        }

        // to simulate the car's step.
        sync_sleep(TIME_UNIT);
    }
}

// endregion private functions