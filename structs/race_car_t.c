#include <stdlib.h>
#include <stdio.h>
#include "../global.h"
#include "../util/error_handler.h"

race_car_t * race_car(race_team_t * team, int car_id, float consumption, float speed, float reliability) {
    race_car_t * new;

    if ((new = (race_car_t *) calloc(1, sizeof(race_car_t))) == NULL) {
        throw_error(ERROR_MEMORY_ALLOCATION, RACE_CAR);
    }

    new->car_id = car_id;
    new->team = team;
    new->consumption = consumption;
    new->speed = speed;
    new->reliability = reliability;

    new->remaining_fuel = mem_struct->cfg->fuel_tank_capacity;
    new->status = CAR_RACE_STATE;

    return new;
}

char * race_car_to_string(race_car_t * car) {
    if (car == NULL) {
        return NULL;
    }

    char * buffer = (char *) malloc(MAX_BUFFER_SIZE * sizeof(char));

    snprintf(buffer, MAX_BUFFER_SIZE * sizeof(char), "RACE CAR NO: %d, "
                                                      "TEAM: %s, "
                                                      "CONSUMPTION: %.2f, "
                                                      "SPEED: %.2f, "
                                                      "RELIABILITY: %.2f, ",
                                                      car->car_id,
                                                      car->team->team_name,
                                                      car->consumption,
                                                      car->speed,
                                                      car->reliability);

    return buffer;
}