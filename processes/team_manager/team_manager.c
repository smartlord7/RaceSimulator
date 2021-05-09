/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include <asm-generic/errno.h>
#include <errno.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "pthread.h"
#include "../../structs/malfunction/malfunction_t.h"
#include "../../util/process_manager/process_manager.h"
#include "../../ipcs/message_queue/msg_queue.h"
#include "team_manager.h"
#include "../../util/numbers/numbers.h"

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
_Noreturn static void manage_box(race_box_t * box);
static void * race_car_worker(void * data);
static void simulate_car(race_car_t * car);

// endregion private functions prototypes

// region public functions

void team_manager(void * data){
    race_team_t * team = (race_team_t *) data;
    DEBUG_MSG(PROCESS_RUN, DEBUG_LEVEL_ENTRY, team->team_name)

    pthread_t car_threads[MAX_MAX_CARS_PER_TEAM];
    race_car_t * temp_car;
    int i, temp_num_cars;

    i = 0, temp_num_cars = config.max_cars_per_team;
    team->num_cars = temp_num_cars;

    init_mutex(&team->team_box.car_in, true);
    init_mutex(&team->team_box.available, true);
    init_cond(&team->team_box.cond, true);
    team->team_box.team = team;

    while (i < temp_num_cars) {
        temp_car = race_car(team, ++shm->total_num_cars, 0.02f, 20, 0.8f, 40);
        snprintf(temp_car->name, MAX_LABEL_SIZE, "%s_%d", RACE_CAR, temp_car->car_id);
        shm->race_cars[team->team_id][i] = * temp_car;

        DEBUG_MSG(THREAD_RUN, DEBUG_LEVEL_ENTRY, temp_car->name)
        create_thread(temp_car->name, &car_threads[i], race_car_worker, &shm->race_cars[team->team_id][i]);

        i++;
    }

    manage_box(&team->team_box);

    wait_threads(team->num_cars, car_threads);

    DEBUG_MSG(PROCESS_EXIT, DEBUG_LEVEL_ENTRY, TEAM_MANAGER)

    exit(EXIT_SUCCESS);
}
// endregion public functions

// region private functions

_Noreturn void manage_box(race_box_t * box) {
    race_car_t * car = NULL;
    uint fuel_time, repair_time;

    fuel_time = tu_to_msec(REFUEL_TIME_FACTOR * config.time_units_per_sec);
    box->current_car = NULL;
    box->state = FREE;

    while (true) {

        lock_mutex(&box->car_in);
        while (box->current_car == NULL) {
            wait_cond(&box->cond, &box->car_in);
        }
        unlock_mutex(&box->car_in);

        box->state = OCCUPIED;

        car = box->current_car;

        DEBUG_MSG(CAR_FIX, DEBUG_LEVEL_EVENT, box->team->team_id, car->car_id)

        repair_time = (uint) random_int(config.min_repair_time, config.max_repair_time);
        ms_sleep(repair_time);

        DEBUG_MSG(CAR_REFUEL, DEBUG_LEVEL_EVENT, box->team->team_id, car->car_id)

        //tim1.tv_sec = fuel_time;
        ms_sleep(fuel_time);

        lock_mutex(&car->mutex);
        car->state = RACE; // Good to go :-)
        notify_cond(&car->start_cond);
        unlock_mutex(&car->mutex);
        box->current_car = NULL;

        box->state = FREE;
        unlock_mutex(&box->available);
    }
}

void * race_car_worker(void * data){
    race_car_t * car = (race_car_t *) data;
    DEBUG_MSG(THREAD_RUN, DEBUG_LEVEL_ENTRY, car->name)

    simulate_car(car);

    DEBUG_MSG(THREAD_EXIT, DEBUG_LEVEL_ENTRY, car->name)

    pthread_exit(EXIT_SUCCESS);
}

void simulate_car(race_car_t * car) {
    init_mutex(&car->mutex, true);
    init_cond(&car->start_cond, true);

    malfunction_t malfunction_msg;
    long int time_step;
    int box_needed;
    float min_fuel1, min_fuel2;

    box_needed = false;
    min_fuel1 = REFUEL_MIN_LAPS1 * config.lap_distance / car->speed * car->consumption;
    min_fuel2 = REFUEL_MIN_LAPS2 * config.lap_distance / car->speed * car->consumption;
    time_step = tu_to_msec(config.time_units_per_sec);


    lock_mutex(&car->mutex);
    while (car->state == NONE) {
        wait_cond(&car->start_cond, &car->mutex);
    }
    unlock_mutex(&car->mutex);

    while(true) {
        DEBUG_MSG(race_car_to_string(car), DEBUG_LEVEL_PARAM, "")

        if (box_needed) {
            race_box_t * box = &car->team->team_box;
            if (pthread_mutex_trylock(&box->available)) {

                set_state(car, IN_BOX);

                lock_mutex(&box->car_in);
                car->team->team_box.current_car = car;
                notify_cond(&box->cond);
                unlock_mutex(&box->car_in);


                lock_mutex(&car->mutex);
                while (car->state == IN_BOX) {
                    wait_cond(&car->start_cond, &car->mutex);
                }
                unlock_mutex(&car->mutex);
                box_needed = false;
            }
        }

        if (rcv_msg(malfunction_q_id, (void *) &malfunction_msg, sizeof(malfunction_msg), car->car_id) > 0) {
            printf("%s\n", malfunction_msg.malfunction_msg);

            set_state(car, SAFETY);
            box_needed = true;

            DEBUG_MSG(CAR_STATE_CHANGE, DEBUG_LEVEL_EVENT, car->car_id, car->state)
        }

        car->remaining_fuel -= car->current_consumption * config.time_units_per_sec;
        car->current_pos += car->current_speed * config.time_units_per_sec;

        DEBUG_MSG(CAR_MOVE, DEBUG_LEVEL_EVENT, car->car_id, car->current_pos)

        if (car->current_pos >= config.lap_distance) {
            car->completed_laps++;
            car->current_pos = 0;

            DEBUG_MSG(CAR_COMPLETE_LAP, DEBUG_LEVEL_EVENT, car->car_id, car->completed_laps)

            if (car->completed_laps == config.laps_per_race) {
                set_state(car, FINISHED);

                DEBUG_MSG(CAR_STATE_CHANGE, DEBUG_LEVEL_EVENT, car->car_id, car->state)

                DEBUG_MSG(CAR_FINISH, DEBUG_LEVEL_EVENT, car->car_id)
            }
        }

        if (car->remaining_fuel <= 0) {
            set_state(car, NON_FIT);

            DEBUG_MSG(CAR_STATE_CHANGE, DEBUG_LEVEL_EVENT, car->car_id, car->state)

            break;
        }

        if (car->remaining_fuel <= min_fuel1 && !box_needed) {
            box_needed = true;
        }

        if (car->remaining_fuel <= min_fuel2) {
            set_state(car, SAFETY);

            DEBUG_MSG(CAR_STATE_CHANGE, DEBUG_LEVEL_EVENT, car->car_id, car->state)
        }

        ms_sleep(time_step);
    }
}

// endregion private functions