/*
 * Authors:
 *  - Joao Filipe Guiomar Artur, 2019217853
 *  - Sancho Amaral Simoes, 2019217590
 *
 * Date of creation: 02/04/2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "../util/exception_handler.h"
#include "race_config_t.h"

#define true 1
#define false 0
#define MAX_BUFFER_SIZE 512

char * race_config_to_string(race_config_t * cfg) {
    char * buffer;

    if ((buffer = (char *) malloc(MAX_BUFFER_SIZE * sizeof(char))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, "BUFFER");
    }

    snprintf(buffer, MAX_BUFFER_SIZE * sizeof(char), "\nTime units per second: %.2ftu\n"
                                                     "Lap distance: %.2fm\n"
                                                     "Number of laps per race: %d\n"
                                                     "Number of teams: %d\n"
                                                     "Max. number of cars per team: %d\n"
                                                     "Malfunction interval: %.2ftu\n"
                                                     "Min. repair time: %.2ftu\n"
                                                     "Max. repair time: %.2ftu\n"
                                                     "Fuel tank capacity: %.2fL\n",
             cfg->time_units_per_sec,
             cfg->lap_distance,
             cfg->laps_per_race,
             cfg->num_teams,
             cfg->max_cars_per_team,
             cfg->malfunction_interval,
             cfg->min_repair_time,
             cfg->max_repair_time,
             cfg->fuel_tank_capacity);

    return buffer;
}