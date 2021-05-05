/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../../util/debug/exception_handler.h"
#include "../../util/process_manager/process_manager.h"
#include "team_manager.h"
#include "../../util/global.h"
#include "../../ipcs/sync/semaphore/sem.h"

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
void * race_car_worker(void * race_car);

// endregion private functions prototypes

// region global variables

//static mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;

// endregion global variables

// region public functions

void team_manager(void * data){
    pthread_t car_threads[MAX_MAX_CARS_PER_TEAM];

    race_team_t * team = (race_team_t *) data;

    DEBUG_MSG(PROCESS_RUN, team->team_name)

    int i = 0, temp_num_cars = shm->cfg->max_cars_per_team;
    team->num_cars = temp_num_cars;

    printf("%s", race_team_to_string(team));

    race_car_t * temp_car = race_car(team, 0, 3.5f, 120, 0.5f, shm->cfg->fuel_tank_capacity);
    char buffer[MAX_LABEL_SIZE];
    snprintf(buffer, MAX_LABEL_SIZE, "%s_%d", RACE_CAR, temp_car->car_id);

    strcpy(temp_car->name, buffer);

    while (i < temp_num_cars) {
        create_thread(temp_car->name, &car_threads[i], race_car_worker, (void *) temp_car);
        i++;
    }

    wait_threads(team->num_cars, car_threads);

    DEBUG_MSG(PROCESS_EXIT, TEAM_MANAGER)

    exit(EXIT_SUCCESS);
}
// endregion public functions


// region private functions

void * race_car_worker(void * race_car){
    race_car_t * car = (race_car_t *) race_car;

    printf("%s", race_car_to_string(car));

    #if DEBUG
    char buffer[MAX_LABEL_SIZE];
    snprintf(buffer, MAX_LABEL_SIZE, "%s_%d", RACE_CAR, car->car_id);

    DEBUG_MSG(THREAD_RUN, buffer)

    DEBUG_MSG(THREAD_EXIT, buffer)

    #endif

    throw_and_stay(NOT_IMPLEMENTED_EXCEPTION, CAR_THREAD);

    pthread_exit(EXIT_SUCCESS);
}

// endregion private functions