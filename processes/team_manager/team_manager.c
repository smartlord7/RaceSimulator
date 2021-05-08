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
#include "../../ipcs/sync/semaphore/sem.h"
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
void * race_car_worker(void * data);
void simulate_car(race_car_t * car);

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

    while (i < temp_num_cars) {
        temp_car = race_car(team, ++shm->total_num_cars, 0.02f, 20, 0.8f, 40);
        snprintf(temp_car->name, MAX_LABEL_SIZE, "%s_%d", RACE_CAR, temp_car->car_id);
        shm->race_cars[team->team_id][i] = * temp_car;

        DEBUG_MSG(THREAD_RUN, DEBUG_LEVEL_ENTRY, temp_car->name)
        create_thread(temp_car->name, &car_threads[i], race_car_worker, &shm->race_cars[team->team_id][i]);

        i++;
    }

    wait_threads(team->num_cars, car_threads);

    DEBUG_MSG(PROCESS_EXIT, DEBUG_LEVEL_ENTRY, TEAM_MANAGER)

    exit(EXIT_SUCCESS);
}
// endregion public functions

// region private functions

void * race_car_worker(void * data){
    race_car_t * car = (race_car_t *) data;
    DEBUG_MSG(THREAD_RUN, DEBUG_LEVEL_ENTRY, car->name)

    simulate_car(car);

    DEBUG_MSG(THREAD_EXIT, DEBUG_LEVEL_ENTRY, car->name)

    pthread_exit(EXIT_SUCCESS);
}

void simulate_car(race_car_t * car) {
    DEBUG_MSG(race_car_to_string(car), DEBUG_LEVEL_PARAM, "")
    set_sh_proc_mutex(&car->mutex);
    init_cond(&car->start_cond, true);

    malfunction_t malfunction_msg;
    long int time_step;
    int box_needed;
    float min_fuel1, min_fuel2;
    struct timespec tim1, tim2;

    box_needed = false;
    min_fuel1 = REFUEL_MIN_LAPS1 * config.lap_distance / car->speed * car->consumption;
    min_fuel2 = REFUEL_MIN_LAPS2 * config.lap_distance / car->speed * car->consumption;
    time_step = tu_to_sec(config.time_units_per_sec);
    tim1.tv_sec = time_step;
    tim2.tv_nsec = 0;


    wait_cond(&car->start_cond, &car->mutex);

    while(true) {
        if (box_needed) {


            /**set_state(car, IN_BOX);
            DEBUG_MSG(CAR_STATE_CHANGE, DEBUG_LEVEL_EVENT, car->car_id, car->state)**/

            // IN BOX
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

        nanosleep(&tim1, &tim2);
    }
}

// endregion private functions