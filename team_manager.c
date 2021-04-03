#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "global.h"
#include "util/process_manager.h"
#include "util/error_handler.h"

static int num_cars;
static race_car_t ** team_cars;
static pthread_t * car_threads;

void * race_car_worker(void * race_car){
    race_car_t car = *((race_car_t *) race_car);

    #if DEBUG
    char buffer[MAX_LABEL_SIZE];
    snprintf(buffer, MAX_LABEL_SIZE, "%s_%d", RACE_CAR, car.car_id);

    S_DEBUG_MSG(RUNNING_THREAD, buffer)

    S_DEBUG_MSG(EXITING_THREAD, buffer)
    #endif

    s_throw_error_end_stay(ERROR_NOT_IMPLEMENTED, CAR_THREAD);

    pthread_exit(EXIT_SUCCESS);
}

void team_manager(void * data){
    race_team_t * team = (race_team_t *) data;

    S_DEBUG_MSG(RUNNING_PROCESS, team->team_name)

    int i = 0, temp_num_cars = mem_struct->cfg->max_cars_per_team;
    num_cars = temp_num_cars;

    if ((car_threads = (pthread_t *) malloc(temp_num_cars * sizeof(pthread_t))) == NULL) {
        s_throw_error_end_exit(ERROR_MEMORY_ALLOCATION, "car threads");
    }

    if ((team_cars = (race_car_t **) malloc(num_cars * sizeof(race_car_t *))) == NULL) {
        s_throw_error_end_exit(ERROR_MEMORY_ALLOCATION, "team race cars");
    }

    race_car_t * temp_car = race_car(team, 0, 3.5f, 120, 0.5f);
    char buffer[MAX_LABEL_SIZE];
    snprintf(buffer, MAX_LABEL_SIZE, "%s_%d", RACE_CAR, temp_car->car_id);

    strcpy(temp_car->name, buffer);

    while (i < temp_num_cars) {
        team_cars[i] = temp_car;
        create_thread(temp_car->name, &car_threads[i], race_car_worker, (void *) temp_car);
        i++;
    }

    kill_all_threads(num_cars, car_threads);

    S_DEBUG_MSG(EXITING_PROCESS, TEAM_MANAGER)

    exit(EXIT_SUCCESS);
}