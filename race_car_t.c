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
#include <assert.h>
#include "strings.h"
#include "exception_handler.h"
#include "race_car_t.h"

// endregion dependencies

// region public functions

race_car_t * race_car(race_team_t * team, int car_id, float consumption, float speed, float reliability, float initial_fuel) {
    race_car_t * new = NULL;

    if ((new = (race_car_t *) calloc(1, sizeof(race_car_t))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, RACE_CAR);
    }

    // TODO field validations in constructor.

    new->team = team;
    new->car_id = car_id;
    new->consumption = consumption;
    new->speed = speed;
    new->reliability = reliability;

    set_state(new, RACE);
    new->remaining_fuel = initial_fuel;

    return new;
}

char * race_car_to_string(race_car_t * race_car) {
    char * buffer = NULL;

    if (race_car == NULL) {
        buffer = string(NULL_STR_SIZE);

        snprintf(buffer, NULL_STR_SIZE * sizeof(char), "NULL");
    } else {
        buffer = string(MAX_BUFFER_SIZE);

        snprintf(buffer, MAX_BUFFER_SIZE * sizeof(char), "\nRACE CAR NO: %d\n"
                                                         "NAME: %s\n"
                                                         "TEAM: %s\n"
                                                         "CONSUMPTION: %.2f\n"
                                                         "SPEED: %.2f\n"
                                                         "RELIABILITY: %.2f\n",
                 race_car->car_id,
                 race_car->name,
                 race_car->team->team_name,
                 race_car->consumption,
                 race_car->speed,
                 race_car->reliability);
    }

    return buffer;
}

void set_state(race_car_t * race_car, race_car_state state) {
    assert(race_car != NULL);

    race_car->state = state;

    switch (state) {

        case RACE:
            race_car->current_speed = race_car->speed;
            race_car->current_consumption = race_car->consumption;

            break;
        case SAFETY:
            race_car->current_consumption = SAFETY_CONSUMPTION_RATIO * race_car->consumption;
            race_car->current_speed = SAFETY_SPEED_RATIO * race_car->speed;

            break;
        case IN_BOX_:
            race_car->current_pos = 0;
            race_car->current_speed = 0;
            race_car->current_consumption = 0;

            break;
        case NON_FIT:
            race_car->current_speed = 0;
            race_car->current_consumption = 0;

            break;
        case FINISHED:
            race_car->current_pos = 0;
            race_car->current_speed = 0;
            race_car->current_consumption = 0;

            break;
        default:
            return;
    }
}

// endregion public functions