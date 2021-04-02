#include "global.h"
#include <stdlib.h>
#include <pthread.h>
#include "util/error_handler.h"

#define CAR_THREAD "CAR_THREAD"

static int num_of_cars;
static race_car_t * team_cars;
static pthread_t * car_threads;

void * race_car_worker(void * race_car){
    race_car_t car = *((race_car_t *) race_car);
    throw_error(ERROR_NOT_IMPLEMENTED, CAR_THREAD);

    pthread_exit(EXIT_SUCCESS);
}

void team_manager(void * data){
    race_team_t * team = (race_team_t *) data;
    int i = 0, temp_num_cars = mem_struct->cfg->max_cars_per_team;
    num_of_cars = temp_num_cars;
    car_threads = (pthread_t *) malloc(temp_num_cars * sizeof(pthread_t));
    race_car_t * temp_car = race_car(team, 0, 3.5f, 120, 0.5f);

    while (i < temp_num_cars) {
        pthread_create(&car_threads[i], NULL, race_car_worker, (void *) temp_car);
        i++;
    }

    i = 0;

    while (i < temp_num_cars) {
        pthread_join(car_threads[i], NULL);
        pthread_kill(car_threads[i], SIGKILL);

        i++;
    }
}

