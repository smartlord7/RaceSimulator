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
#include <pthread.h>
#include <string.h>
#include "global.h"
#include "team_manager.h"
#include "util/ipc_manager.h"
#include "util/process_manager.h"
#include "util/exception_handler.h"

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

static pthread_t * car_threads;
//static pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;

// endregion global variables

// region public functions

void team_manager(void * data){
    race_team_t * team = (race_team_t *) data;

    DEBUG_MSG(RUNNING_PROCESS, team->team_name)

    int i = 0, temp_num_cars = mem_struct->cfg->max_cars_per_team;
    team->num_cars = temp_num_cars;

    if ((car_threads = (pthread_t *) malloc(temp_num_cars * sizeof(pthread_t))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, CAR_THREADS);
    }

    if ((team->cars = (race_car_t **) malloc(team->num_cars * sizeof(race_car_t *))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, TEAM_CARS);
    }

    wait_sem(output_mutex, OUTPUT_MUTEX);

    printf("%s", race_team_to_string(team));

    post_sem(output_mutex, OUTPUT_MUTEX);

    race_car_t * temp_car = race_car(team, 0, 3.5f, 120, 0.5f, mem_struct->cfg->fuel_tank_capacity);
    char buffer[MAX_LABEL_SIZE];
    snprintf(buffer, MAX_LABEL_SIZE, "%s_%d", RACE_CAR, temp_car->car_id);

    strcpy(temp_car->name, buffer);

    while (i < temp_num_cars) {
        team->cars[i] = temp_car;
        create_thread(temp_car->name, &car_threads[i], race_car_worker, (void *) temp_car);
        i++;
    }

    kill_threads(team->num_cars, car_threads);
    free(team->cars);
    free(car_threads);

    DEBUG_MSG(EXITING_PROCESS, TEAM_MANAGER)

    exit(EXIT_SUCCESS);
}
// endregion public functions


// region private functions

void * race_car_worker(void * race_car){
    race_car_t * car = (race_car_t *) race_car;

    wait_sem(output_mutex, OUTPUT_MUTEX);

    printf("%s", race_car_to_string(car));

    post_sem(output_mutex, OUTPUT_MUTEX);

    #if DEBUG
    char buffer[MAX_LABEL_SIZE];
    snprintf(buffer, MAX_LABEL_SIZE, "%s_%d", RACE_CAR, car.car_id);

    DEBUG_MSG(RUNNING_THREAD, buffer)

    DEBUG_MSG(EXITING_THREAD, buffer)

    #endif

    throw_exception_and_stay(NOT_IMPLEMENTED_EXCEPTION, CAR_THREAD);

    pthread_exit(EXIT_SUCCESS);
}

// endregion private functions