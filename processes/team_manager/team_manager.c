/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include <unistd.h>
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
#include "../race_manager/race_manager.h"
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
static void manage_box(race_box_t * box);
static void * race_car_worker(void * data);
static void simulate_car(race_car_t * car);

// endregion private functions prototypes

int unn_pipe_fds[2];

// region public functions

void team_manager(void * data){
    race_team_t * team = (race_team_t *) data;
    DEBUG_MSG(PROCESS_RUN, ENTRY, team->team_name)

    init_mutex(&team->team_box.cond_mutex, true);
    init_mutex(&team->team_box.access_mutex, true);
    init_mutex(&team->team_box.available, true);
    init_mutex(&team->access_mutex, true);
    init_mutex(&team->pipe_mutex, true);
    init_cond(&team->team_box.cond, true);
    close_fd(unn_pipe_fds[0]);
    team->team_box.team = team;
    team->team_box.car_dispatched = true;
    team->num_cars_safety = 0;

    pthread_t car_threads[MAX_MAX_CARS_PER_TEAM];
    int i;
    race_car_t * temp_car;

    // wait for the race to start.
    SYNC
    while (!shm->sync_s.race_running) {
        wait_cond(&shm->sync_s.cond, &shm->sync_s.mutex);
    }
    END_SYNC

    i = 0;
    while (i < team->num_cars) {
        create_thread(temp_car->name, &car_threads[i], race_car_worker, &shm->race_cars[team->team_id][i]);
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

void manage_box(race_box_t * box) {
    // declare the needed variables
    race_car_t * car = NULL;
    race_team_t * team = NULL;
    uint fuel_time, repair_time;

    // initialize the needed variables
    fuel_time = tu_to_msec(REFUEL_TIME_FACTOR * config.time_units_per_sec);
    team = box->team;
    box->current_car = NULL;
    box->car_dispatched = false;
    box->state = FREE;

    while (shm->sync_s.race_running) {
        // waits for an incoming car or for the number of cars in SAFETY mode to be different of 0.
        SYNC_BOX_COND
        while (box->current_car == NULL && team->num_cars_safety == 0 && shm->sync_s.race_running) {
            wait_cond(&box->cond, &box->cond_mutex);
        }
        END_SYNC_BOX_COND

        if (!shm->sync_s.race_running) {
            return;
        }

        if (team->num_cars_safety > 0) {

            DEBUG_MSG(BOX_RESERVE, EVENT, team->team_name)

            SYNC_BOX
            box->state = RESERVED;
            END_SYNC_BOX

            while (team->num_cars_safety > 0 && shm->sync_s.race_running) {

                SYNC_BOX_COND
                while (box->current_car == NULL && shm->sync_s.race_running) {
                    wait_cond(&box->cond, &box->cond_mutex);
                }
                END_SYNC_BOX_COND

                if (!shm->sync_s.race_running) {
                    SYNC_CAR_COND
                    box->car_dispatched = true;
                    notify_cond(&car->cond);
                    END_SYNC_CAR_COND

                    return;
                }

                car = box->current_car;

                DEBUG_MSG(BOX_CAR_ARRIVE, EVENT, team->team_name, car->name)

                DEBUG_MSG(CAR_FIX, EVENT, team->team_name, car->name)

                repair_time = (uint) random_int(tu_to_msec(config.min_repair_time), tu_to_msec(config.max_repair_time));
                ms_sleep(repair_time);

                if (!shm->sync_s.race_running) {
                    SYNC_CAR_COND
                    box->car_dispatched = true;
                    notify_cond(&car->cond);
                    END_SYNC_CAR_COND

                    return;
                }

                DEBUG_MSG(CAR_REFUEL, EVENT, team->team_name, car->name)

                ms_sleep(fuel_time);

                SYNC_CAR_COND
                box->car_dispatched = true;
                notify_cond(&car->cond);
                END_SYNC_CAR_COND

                box->current_car = NULL;

                unlock_mutex(&box->available);

                SYNC_TEAM
                team->num_cars_safety--;
                END_SYNC_TEAM
            }

            SYNC_BOX
            box->state = FREE;
            END_SYNC_BOX

        } else {
            if (!shm->sync_s.race_running) {
                SYNC_CAR_COND
                box->car_dispatched = true;
                notify_cond(&car->cond);
                END_SYNC_CAR_COND

                return;
            }

            DEBUG_MSG(BOX_CAR_ARRIVE, EVENT, box->team->team_name, car->name)

            car = box->current_car;
            box->state = OCCUPIED;

            DEBUG_MSG(CAR_REFUEL, EVENT, box->team->team_name, car->name)

            ms_sleep(fuel_time);

            SYNC_CAR_COND
            box->car_dispatched = true;
            notify_cond(&car->cond);
            END_SYNC_CAR_COND

            SYNC_BOX
            box->state = FREE;
            END_SYNC_BOX

            box->current_car = NULL;

            unlock_mutex(&box->available);
        }
    }
}

void * race_car_worker(void * data){
    race_car_t * car = (race_car_t *) data;
    DEBUG_MSG(THREAD_RUN, ENTRY, car->name)

    simulate_car(car);

    DEBUG_MSG(THREAD_EXIT, ENTRY, car->name)

    pthread_exit(EXIT_SUCCESS);
}

void simulate_car(race_car_t * car) {
    // declare the needed variables
    long int time_step;
    int box_needed;
    float fuel_per_lap, min_fuel1, min_fuel2;
    malfunction_t malfunction_msg;
    race_car_state_change_t car_state_change;
    race_box_t * box = NULL;
    race_team_t * team = NULL;

    // initialize the needed variables.
    box_needed = false;
    fuel_per_lap = config.lap_distance / car->speed * car->consumption;
    min_fuel1 = REFUEL_MIN_LAPS1 * fuel_per_lap;
    min_fuel2 = REFUEL_MIN_LAPS2 * fuel_per_lap;
    time_step = tu_to_msec(config.time_units_per_sec);
    box = &car->team->team_box;
    team = car->team;
    car_state_change.team_id = car->team->team_id;
    car_state_change.car_id = car->car_id;

    // the car is ready to race.
    CHANGE_CAR_STATE(RACE);

    // the car simulation itself.
    while (true) {
        DEBUG_MSG(race_car_to_string(car), PARAM, "")

        car_state_change.malfunctioning = false;

        // try to gain access to the box, if needed.
        // the car needs to access the box if the following conditions, in the presented short circuit order, are satisfied:
        // - it needs to refuel or to be fixed
        // - it is not about to complete the last lap (why refueling / fixing if its about to cross the finish line?).
        // - it is one step or less closer of the position = 0m (where the box is located).
        // - if the box is reserved and the car is in safety mode or the box is free.
        // - if the car can gain access to the box's lock.
        SYNC_BOX
        if (box_needed && car->completed_laps != config.laps_per_race - 1 && car_close_to_box &&
                ((box->state == RESERVED && car->state == SAFETY) || box->state == FREE) && pthread_mutex_trylock(&box->available) == 0) {
            END_SYNC_BOX

            // spend the needed fuel to reach the box. If the car's position = 0 then the spent fuel is also 0.
            if (car->current_pos != 0) {
                car->remaining_fuel -= (((config.lap_distance - car->current_pos) / car->current_speed) * car->current_consumption);
            }

            // if the car has reached an illegal level of fuel (aka less than 0) to reach the box then it means actually it had no fuel to reach the box.
            if (car->remaining_fuel < 0) {

                // the car's journey has ended :(
                CHANGE_CAR_STATE(DISQUALIFIED);
                generate_log_entry(I_CAR_RAN_OUT_OF_FUEL, car);

                // unlock the previously locked box lock.
                unlock_mutex(&box->available);

                break;
            }

            // the car reached the box and now changed its state to IN_BOX.
            CHANGE_CAR_STATE(IN_BOX);

            SYNC_CAR
            car->num_box_stops++;
            if (car->remaining_fuel <= min_fuel2) {
                car->num_refuels++; // increment the number of refuels dont till now by the car.
                generate_log_entry(I_BOX_REFUEL, car);
            } else if (car->state == SAFETY) { // if the car is SAFETY mode but hasn't reached the min fuel. That means the car is malfunctioning.
                car->num_malfunctions++; // increment the number of malfunctions the car had.
                generate_log_entry(I_BOX_MALFUNCTION, car);
            }
            END_SYNC_CAR


            // notify the box that a new car has arrived.
            SYNC_BOX
            box->current_car = car;
            notify_cond(&box->cond);
            END_SYNC_BOX

            // wait for the box notification that the work on the car is done.
            SYNC_CAR_COND
            while (box->car_dispatched == false) {
                wait_cond(&car->cond, &car->cond_mutex);
            }
            END_SYNC_CAR_COND

            if (!shm->sync_s.race_running) {
                END_SYNC_BOX
                exit_thread();
            }

            // the car is now ready to race again;
            car->remaining_fuel = config.fuel_tank_capacity;

            CHANGE_CAR_STATE(RACE);
            generate_log_entry(I_BOX_LEFT, car);

            // the car needs the box no more.
            box_needed = false;
        }
        END_SYNC_BOX

        // check if the car has some malfunction.
        if (rcv_msg(malfunction_q_id, (void *) &malfunction_msg, sizeof(malfunction_msg), car->car_id) > 0) {
            DEBUG_MSG(malfunction_msg.malfunction_msg, EVENT, "");

            // if a malfunction is detected, the car's state changes to SAFETY.
            car_state_change.malfunctioning = true;
            CHANGE_CAR_STATE(SAFETY);

            // notify the box that at least one of the team's cars is in SAFETY mode.
            SYNC_TEAM
            team->num_cars_safety++;
            notify_cond(&box->cond);
            END_SYNC_TEAM

            // the car needs to access the box.
            box_needed = true;
        }

        // check if the car is about to cross pos = 0.
        if (car->current_pos + car->current_speed * config.time_units_per_sec >= config.lap_distance) {

            // consume the needed fuel to reach the finish line. If the car is in position 0 then the spent fuel is also 0.
            if (car->current_pos != 0) {
                car->remaining_fuel -= (((config.lap_distance - car->current_pos) / car->current_speed) * car->current_consumption);
            }

            // if the car has reached an illegal level of fuel (aka less than 0) to reach pos = 0 then it means actually it had no sufficient fuel to reach pos = 0.
            if (car->remaining_fuel <= 0) {

                // the car's journey has ended :(
                CHANGE_CAR_STATE(DISQUALIFIED);

                break;
            }

            // increase the number of completed laps.
            car->completed_laps++;
            // reset the car's position to 0.
            car->current_pos = 0;

            DEBUG_MSG(CAR_COMPLETE_LAP, EVENT, car->name, car->completed_laps)

            if (!shm->sync_s.race_running) {
                exit_thread();
            }

            // check if the car has completed all the race laps.
            if (car->completed_laps == config.laps_per_race) {

                // change the car's state to FINISHED since it has reached the race's end.
                CHANGE_CAR_STATE(FINISH);

                DEBUG_MSG(CAR_FINISH, EVENT, car->name)

                break;
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

            break;
        }

        // check if the car has fuel to at least run for 4 more laps.
        if (car->remaining_fuel <= min_fuel1 && !box_needed) {

            // the car needs to be refueled since it has insufficient fuel for more 4 laps.
            box_needed = true;
        }

        // check if the car has reached a fuel critical level.
        if (car->remaining_fuel <= min_fuel2 && car->state != SAFETY) {

            // the car needs to be refueled since it has insufficient fuel for more 2 laps.
            box_needed = true;

            // the car must assume SAFETY mode.
            CHANGE_CAR_STATE(SAFETY);

            // notify the box that another car is in SAFETY mode.
            SYNC_TEAM
            team->num_cars_safety++;
            notify_cond(&box->cond);
            END_SYNC_TEAM
        }

        // sleep to simulate the car's step.
        ms_sleep(time_step);
    }
}

// endregion private functions