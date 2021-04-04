#include <stdio.h>
#include <stdlib.h>
#include "../global.h"

char * race_config_to_string(race_config_t * cfg) {
    char * buffer = (char *) malloc(MAX_BUFFER_SIZE * sizeof(char));

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