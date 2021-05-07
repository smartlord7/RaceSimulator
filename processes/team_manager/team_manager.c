/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "pthread.h"
#include "unistd.h"
#include "../../structs/malfunction/malfunction_t.h"
#include "../../util/process_manager/process_manager.h"
#include "../../util/global.h"
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

// endregion private functions prototypes

// region public functions

void team_manager(void * data){
    pthread_t car_threads[MAX_MAX_CARS_PER_TEAM];
    race_team_t * team = (race_team_t *) data;
    race_car_t * temp_car;
    int i, temp_num_cars;

    DEBUG_MSG(PROCESS_RUN, team->team_name)

    i = 0, temp_num_cars = config.max_cars_per_team;
    team->num_cars = temp_num_cars;

    temp_car = race_car(team, 0, 3.5f, 120, 0.80f, config.fuel_tank_capacity);

    char buffer[MAX_LABEL_SIZE];
    snprintf(buffer, MAX_LABEL_SIZE, "%s_%d", RACE_CAR, temp_car->car_id);

    strcpy(temp_car->name, buffer);

    wait_condition_change();

    while (i < temp_num_cars) {
        shm->race_cars[team->team_id][i] = * temp_car;
        SYNC
        shm->race_cars[team->team_id][i].car_id = ++shm->total_num_cars;
        END_SYNC

        create_thread(temp_car->name, &car_threads[i], race_car_worker, &shm->race_cars[team->team_id][i]);

        i++;
    }

    wait_threads(team->num_cars, car_threads);

    DEBUG_MSG(PROCESS_EXIT, TEAM_MANAGER)

    exit(EXIT_SUCCESS);
}
// endregion public functions

// region private functions

void * race_car_worker(void * data){
    race_car_t * car = (race_car_t *) data;
    malfunction_t malfunction_msg;
    uint time_step;

    DEBUG_MSG(THREAD_RUN, car->name)

    time_step = tu_to_usec(1);

    while (true) {
        if (rcv_msg(malfunction_msg_q_id, &malfunction_msg, sizeof(malfunction_t), car->car_id) > 0) {

            SYNC
            set_state(car, SAFETY);
            END_SYNC

            printf("%s\n", malfunction_msg.malfunction_msg);
        }

        usleep(time_step);
    }

    DEBUG_MSG(THREAD_EXIT, car->name)

    pthread_exit(EXIT_SUCCESS);
}

// endregion private functions