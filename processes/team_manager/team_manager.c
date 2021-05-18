/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include "unistd.h"
#include "signal.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "pthread.h"
#include "../../structs/malfunction/malfunction_t.h"
#include "../../util/process_manager/process_manager.h"
#include "../../ipcs/message_queue/msg_queue.h"
#include "team_manager.h"
#include "../../util/numbers/numbers.h"
#include "../../util/file/file.h"
#include "../../race_helpers/log_generator/log_generator.h"

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

static void * race_car_worker(void *data);

static void simulate_car(race_car_t *car);

// endregion private functions prototypes

int unn_pipe_fds[2];

// region public functions

void team_manager(void *data) {
    race_team_t *team = (race_team_t *) data;
    DEBUG_MSG(PROCESS_RUN, ENTRY, team->team_name)

    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

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
    wait_race_start();

    i = 0;
    while (i < team->num_cars) {
        create_thread(shm->race_cars[team->team_id][i].name, &car_threads[i], race_car_worker, &shm->race_cars[team->team_id][i]);
        i++;
    }

    DEBUG_MSG(FUNCTION_RUN, ENTRY, TEAM_BOX)

    manage_box(&team->team_box);

    DEBUG_MSG(FUNCTION_EXIT, ENTRY, TEAM_BOX)

    wait_threads(team->num_cars, car_threads);

    close_fd(unn_pipe_fds[1]);

    DEBUG_MSG(PROCESS_EXIT, ENTRY, team->team_name)
}
// endregion public functions

// region private functions

void manage_box(race_box_t *box) {
    // declare the needed variables
    race_car_t *car = NULL;
    race_team_t *team = NULL;
    int repair_time;

    // initialize the needed variables
    team = box->team;
    box->current_car = NULL;
    box->car_dispatched = false;

    while (true) {

        box->state = FREE;
        generate_log_entry(BOX_STATE_CHANGE, box, NULL);

        // waits for an incoming car or for the number of cars in SAFETY mode to be different of 0.
        SYNC_BOX_COND
        while (shm->sync_s.race_running && team->num_cars_safety == 0 && box->current_car == NULL) {
            wait_cond(&box->cond, &box->mutex);
        }

        if (!shm->sync_s.race_running) {
            END_SYNC_BOX_COND
            return;
        }
        END_SYNC_BOX_COND

        if (team->num_cars_safety > 0) {

            box->state = RESERVED;
            generate_log_entry(BOX_STATE_CHANGE, box, NULL);

            while (true) {
                SYNC_BOX_COND
                while (shm->sync_s.race_running && team->num_cars_safety > 0 && box->current_car == NULL) {
                    wait_cond(&box->cond, &box->mutex);
                }
                if (team->num_cars_safety == 0) {
                    END_SYNC_BOX_COND
                    break;
                }
                END_SYNC_BOX_COND

                if (!shm->sync_s.race_running) {
                    if (box->current_car != NULL) {
                        SYNC_CAR_COND
                        box->car_dispatched = true;
                        notify_cond(&car->cond);
                        END_SYNC_CAR_COND
                    }

                    return;
                }

                car = box->current_car;
                repair_time = random_int(config.min_repair_time, config.max_repair_time);

                generate_log_entry(BOX_FIX, box, NULL);
                sync_sleep(repair_time);

                if (!shm->sync_s.race_running) {

                    SYNC_CAR_COND
                    box->car_dispatched = true;
                    notify_cond(&car->cond);
                    END_SYNC_CAR_COND

                    return;
                }

                generate_log_entry(BOX_REFUEL, box, NULL);
                sync_sleep(REFUEL_TIME);

                SYNC_CAR_COND
                box->car_dispatched = true;
                notify_cond(&car->cond);
                END_SYNC_CAR_COND
                generate_log_entry(BOX_LEAVE, box, NULL);

                box->current_car = NULL;
            }
        } else if (box->current_car != NULL) {
            car = box->current_car;

            box->state = OCCUPIED;
            generate_log_entry(BOX_STATE_CHANGE, box, NULL);

            generate_log_entry(BOX_REFUEL, box, NULL);
            sync_sleep(REFUEL_TIME);

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
    race_car_t *car = (race_car_t *) data;
    DEBUG_MSG(THREAD_RUN, ENTRY, car->name)

    simulate_car(car);

    DEBUG_MSG(THREAD_EXIT, ENTRY, car->name)

    pthread_exit(EXIT_SUCCESS);
}

void simulate_car(race_car_t *car) {
    // declare the needed variables
    int box_needed;
    float fuel_per_lap, min_fuel1, min_fuel2;
    malfunction_t malfunction = {0};
    race_car_state_change_t car_state_change = {0};
    race_box_t *box = NULL;
    race_team_t *team = NULL;

    // initialize the needed variables.
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
        // - it needs to refuel or to be fixed
        // - it is not about to complete the last lap (why refueling / fixing if its about to cross the finish line?).
        // - it is one step or less closer of the position = 0m (where the box is located).
        // - if the box is reserved and the car is in safety mode or the box is free.
        // - if the car can gain access to the box's lock.
        if (shm->sync_s.race_running && box_needed && car->completed_laps != config.laps_per_race - 1 && car_close_to_box &&
                ((box->state == RESERVED && car->state == SAFETY) || box->state == FREE) && pthread_mutex_trylock(&box->available) == 0) {

            // spend the needed fuel to reach the box. If the car's position = 0 then the spent fuel is also 0.
            if (car->current_pos != 0) {
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

                return;
            }

            // the car reached the box and now changed its state to IN_BOX.
            CHANGE_CAR_STATE(IN_BOX);

            SYNC
            car->num_box_stops++;
            END_SYNC

            // notify the box that a new car has arrived.
            SYNC_BOX_COND
            box->current_car = car;
            notify_cond(&box->cond);
            END_SYNC_BOX_COND

            // wait for the box notification that the work on the car is done.
            SYNC_CAR_COND
            while (box->car_dispatched == false && shm->sync_s.race_running) {
                wait_cond(&car->cond, &car->cond_mutex);
            }
            END_SYNC_CAR_COND

            unlock_mutex(&box->available);

            // the car is now ready to race again
            car->remaining_fuel = config.fuel_tank_capacity;

            car->num_refuels++;

            CHANGE_CAR_STATE(RACE);

            if (!shm->sync_s.race_running || shm->sync_s.race_interrupted) {
                CHANGE_CAR_STATE(FINISH);
                generate_log_entry(CAR_FINISH, car, NULL);
                exit_thread();
            }

            // the car is now ready to race again;
            car->remaining_fuel = config.fuel_tank_capacity;

            CHANGE_CAR_STATE(RACE);

            // the car needs the box no more.
            box_needed = false;
        }

        // check if the car has some malfunction.
        if (rcv_msg(malfunction_q_id, (void *) &malfunction, sizeof(malfunction), car->car_id) > 0) {
            DEBUG_MSG(malfunction.description, EVENT, "");

            // if a malfunction is detected, the car's state changes to SAFETY.
            car_state_change.malfunctioning = true;
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

                break;
            }

            // increase the number of completed laps.
            SYNC
            car->completed_laps++;
            END_SYNC
            // reset the car's position to 0.
            car->current_pos = 0;

            generate_log_entry(CAR_COMPLETE_LAP, car, NULL);

            if (shm->sync_s.race_interrupted) {
                CHANGE_CAR_STATE(FINISH);
                generate_log_entry(CAR_FINISH, car, NULL);
                exit_thread();
            }

            // check if the car has completed all the race laps.
            if (car->completed_laps == config.laps_per_race) {

                // change the car's state to FINISHED since it has reached the race's end.
                CHANGE_CAR_STATE(FINISH);
                generate_log_entry(CAR_FINISH, car, NULL);

                return;
            }

            continue;
        }

        // spend the needed fuel
        car->remaining_fuel -= car->current_consumption;
        // do a discrete spatial step :-)
        car->current_pos += car->current_speed;

        DEBUG_MSG(CAR_MOVE, EVENT, car->name, car->current_pos)

        // check if the car's fuel as reached an end.
        if (car->remaining_fuel <= 0) {

            // the car's journey has ended :(
            CHANGE_CAR_STATE(DISQUALIFIED);
            generate_log_entry(CAR_OUT_OF_FUEL, car, NULL);

            return;
        }

        // check if the car has fuel to at least run for 4 more laps.
        if (!box_needed && car->remaining_fuel <= min_fuel1) {

            // the car needs to be refueled since it has insufficient fuel for more 4 laps.
            box_needed = true;
        }

        // check if the car has reached a fuel critical level.
        if (car->state != SAFETY && car->remaining_fuel <= min_fuel2) {

            // the car needs to be refueled since it has insufficient fuel for more 2 laps.
            box_needed = true;

            // the car must assume SAFETY mode.
            CHANGE_CAR_STATE(SAFETY);
        }

        // to simulate the car's step.
        sync_sleep(1);
    }

}

// endregion private functions