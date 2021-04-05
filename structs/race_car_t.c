/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 02/04/2021
*/

// region dependencies

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "race_car_t.h"
#include "../util/strings.h"
#include "../util/exception_handler.h"

// endregion dependencies

// region public functions

race_car_t * race_car(race_team_t * team, int car_id, float consumption, float speed, float reliability, float initial_fuel) {
    race_car_t * new = NULL;

    if ((new = (race_car_t *) calloc(1, sizeof(race_car_t))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, RACE_CAR);
    }

    new->car_id = car_id;
    new->team = team;
    new->consumption = consumption;
    new->speed = speed;
    new->reliability = reliability;

    new->remaining_fuel = initial_fuel;
    new->state = RACE;

    return new;
}

char * race_car_to_string(race_car_t * race_car) {
    char * buffer = NULL;

    if (race_car == NULL) {
        buffer = string(NULL_STR_SIZE);

        snprintf(buffer, NULL_STR_SIZE * sizeof(char), "NULL");
    } else {
        buffer = string(MAX_BUFFER_SIZE);

        snprintf(buffer, MAX_BUFFER_SIZE * sizeof(char), "RACE CAR NO: %d, "
                                                         "NAME: %s, "
                                                         "TEAM: %s, "
                                                         "CONSUMPTION: %.2f, "
                                                         "SPEED: %.2f, "
                                                         "RELIABILITY: %.2f, ",
                 race_car->car_id,
                 race_car->name,
                 race_car->team->team_name,
                 race_car->consumption,
                 race_car->speed,
                 race_car->reliability);
    }

    return buffer;
}

// endregion public functions