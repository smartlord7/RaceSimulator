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
int check_min_fuel(race_car_t * car, float min_fuel);

// endregion private functions prototypes

// region public functions

void team_manager(void * data){
    pthread_t car_threads[MAX_MAX_CARS_PER_TEAM];
    race_team_t * team = (race_team_t *) data;
    race_car_t * temp_car;
    int i, temp_num_cars;

    DEBUG_MSG(PROCESS_RUN, DEBUG_LEVEL_ENTRY, team->team_name)

    i = 0, temp_num_cars = config.max_cars_per_team;
    team->num_cars = temp_num_cars;

    temp_car = race_car(team, 0, 3.5f, 20, 0.80f, config.fuel_tank_capacity);

    char buffer[MAX_LABEL_SIZE];
    snprintf(buffer, MAX_LABEL_SIZE, "%s_%d", RACE_CAR, temp_car->car_id);

    strcpy(temp_car->name, buffer);

    while (i < temp_num_cars) {
        shm->race_cars[team->team_id][i] = * temp_car;
        shm->race_cars[team->team_id][i].car_id = ++shm->total_num_cars;

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

    set_sh_mutex(&car->mutex);
    malfunction_t malfunction_msg;
    uint time_step;
    int box_needed = false;
    float min_fuel = MIN_FUEL_LAPS * config.lap_distance / car->speed * car->consumption;
    time_step = tu_to_nsec(config.time_units_per_sec);

    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = time_step;

    DEBUG_MSG(race_car_to_string(car), DEBUG_LEVEL_PARAM, "")

    monitor_wait_change(RACE_START_MONITOR);

    while(car->state != FINISHED) {
        if (box_needed && car->team->team_box->state == FREE) {
            car->team->team_box->state = OCCUPIED;

            // TODO signal team manager that a new car has arrived
        }

        if (rcv_msg(malfunction_msg_q_id, &malfunction_msg, sizeof(malfunction_t), car->car_id) > 0) {

            //set_state(car, SAFETY);
            //DEBUG_MSG(malfunction_msg.malfunction_msg, DEBUG_LEVEL_EVENT, "")
        }

        car->remaining_fuel -= car->current_consumption * (float) time_step;

        box_needed = check_min_fuel(car, min_fuel);

        car->current_pos += car->current_speed * config.time_units_per_sec;

        DEBUG_MSG(CAR_MOVE, DEBUG_LEVEL_EVENT, car->car_id, car->current_pos)

        if (car->current_pos >= config.lap_distance) {
            car->completed_laps++;
            car->current_pos = 0;
        }

        if (car->completed_laps == config.laps_per_race) {
            car->state = FINISHED;
        }

        nanosleep(&tim, &tim2);
    }

    DEBUG_MSG(CAR_FINISH, DEBUG_LEVEL_EVENT, "")
    DEBUG_MSG(THREAD_EXIT, DEBUG_LEVEL_ENTRY, car->name)

    pthread_exit(EXIT_SUCCESS);
}

int check_min_fuel(race_car_t * car, float min_fuel) {
    if (car->remaining_fuel < min_fuel) {
        set_state(car, SAFETY);
        return true;
    }

    return false;
}

// endregion private functions