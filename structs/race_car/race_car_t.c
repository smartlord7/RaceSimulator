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
#include "../../util/strings/strings.h"
#include "../../util/exception_handler/exception_handler.h"
#include "race_car_t.h"
#include "../../util/global.h"

int check_unique_id(int car_id, race_car_t * car_data);

// endregion dependencies

// region public functions

race_car_t *
race_car(race_team_t *team, char * car_name, int car_id, float consumption, float speed, float reliability, float initial_fuel) {
    race_car_t *new = NULL;

    if ((new = (race_car_t *) calloc(1, sizeof(race_car_t))) == NULL) {
        throw_and_exit(MEMORY_ALLOCATE_EXCEPTION, RACE_CAR);
    }

    new->team = team;
    strcpy(new->name, car_name);
    new->car_id = car_id;
    new->consumption = consumption;
    new->speed = speed;
    new->reliability = reliability;
    new->remaining_fuel = initial_fuel;

    return new;
}

char * race_car_to_string(race_car_t *race_car) {
    char * buffer = NULL;

    if (race_car == NULL) {
        buffer = string(NULL_STR_SIZE);

        snprintf(buffer, NULL_STR_SIZE * sizeof(char), "NULL");
    } else {
        buffer = string(BUF_SIZE);

        snprintf(buffer, BUF_SIZE * sizeof(char),
                 "\n-----INITIAL PROPS-----\n"
                 "RACE CAR NO: %d\n"
                 "NAME: %s\n"
                 "TEAM: %s\n"
                 "CONSUMPTION: %.2f\n"
                 "SPEED: %.2f\n"
                 "RELIABILITY: %.2f\n"
                 "-----REAL-TIME PROPS-----\n"
                 "CURRENT STATE: %d\n"
                 "CURRENT POSITION: %.2f\n"
                 "CURRENT CONSUMPTION: %.2f\n"
                 "CURRENT SPEED: %.2f\n",
                 race_car->car_id,
                 race_car->name,
                 race_car->team->team_name,
                 race_car->consumption,
                 race_car->speed,
                 race_car->reliability,
                 race_car->state,
                 race_car->current_pos,
                 race_car->current_consumption,
                 race_car->current_speed);
    }

    return buffer;
}

char * race_car_stats_string(race_car_t * car) {
    char * buffer = NULL;

    if (car == NULL) {
        buffer = string(NULL_STR_SIZE);

        snprintf(buffer, NULL_STR_SIZE * sizeof(char), "NULL");
    } else {
        buffer = string(BUF_SIZE);

        snprintf(buffer, BUF_SIZE * sizeof(char), "%d    %d (%s)      %d      %d",
                 car->car_id,
                 car->team->team_id,
                 car->team->team_name,
                 car->completed_laps,
                 car->num_box_stops);
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
        case IN_BOX:
        case NON_FIT:
        case FINISH:
            race_car->current_speed = 0;
            race_car->current_consumption = 0;
        default:
            return;
    }
}

// endregion public functions