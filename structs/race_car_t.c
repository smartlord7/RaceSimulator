/* Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* Authors:
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* Date of creation: 02/04/2021
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../util/exception_handler.h"
#include "race_car_t.h"

#define true 1
#define false 0
#define MAX_BUFFER_SIZE 512

race_car_t * race_car(race_team_t * team, int car_id, float consumption, float speed, float reliability, float initial_fuel) {
    race_car_t * new;

    if ((new = (race_car_t *) calloc(1, sizeof(race_car_t))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, "BUFFER");
    }

    new->car_id = car_id;
    new->team = team;
    new->consumption = consumption;
    new->speed = speed;
    new->reliability = reliability;

    new->remaining_fuel = initial_fuel;
    new->status = CAR_RACE_STATE;

    return new;
}

char * race_car_to_string(race_car_t * car) {
    if (car == NULL) {
        return NULL;
    }

    char * buffer = (char *) malloc(MAX_BUFFER_SIZE * sizeof(char));

    snprintf(buffer, MAX_BUFFER_SIZE * sizeof(char), "RACE CAR NO: %d, "
                                                     "NAME: %s, "
                                                      "TEAM: %s, "
                                                      "CONSUMPTION: %.2f, "
                                                      "SPEED: %.2f, "
                                                      "RELIABILITY: %.2f, ",
                                                      car->car_id,
                                                      car->name,
                                                      car->team->team_name,
                                                      car->consumption,
                                                      car->speed,
                                                      car->reliability);

    return buffer;
}