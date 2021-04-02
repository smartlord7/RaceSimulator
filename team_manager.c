#include "global.h"
#include "structs/shared_memory.h"
#include <stdlib.h>
#include <pthread.h>
#include "util/error_handler.h"

#define CAR_THREAD "CAR THREAD"

int start_data();
void * create_car(void * car_info){
    race_car car = *((race_car *) car_info);
    throw_error(ERROR_NOT_IMPLEMENTED, CAR_THREAD);
}


int max_num_of_cars, num_of_cars, * cars_id;
pthread_t * my_cars;


int team_manager(){

}

int start_data(){
    max_num_of_cars = mem_struct->cfg->max_cars_per_team;
    cars_id = malloc(max_num_of_cars * sizeof(int));


}

