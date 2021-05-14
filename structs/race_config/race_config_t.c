/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include <stdio.h>
#include <stdlib.h>
#include "../../util/strings/strings.h"
#include "../../util/exception_handler/exception_handler.h"
#include "race_config_t.h"
#include "../race_car/race_car_t.h"

// endregion dependencies

// region public functions

race_config_t * race_config(float time_units_per_sec, float lap_distance, float malfunction_interval, float min_repair_time,
                          float max_repair_time, float fuel_tank_capacity, int laps_per_race, int num_teams, int max_cars_per_team) {

    race_config_t * new = NULL;

    if ((new = (race_config_t *) calloc(1, sizeof(race_config_t))) == NULL) {
        throw_and_exit(MEMORY_ALLOCATE_EXCEPTION, RACE_CONFIG);
    }

    new->time_units_per_sec = time_units_per_sec;
    new->lap_distance = lap_distance;
    new->malfunction_interval = malfunction_interval;
    new->min_repair_time = min_repair_time;
    new->max_repair_time = max_repair_time;
    new->fuel_tank_capacity = fuel_tank_capacity;
    new->laps_per_race = laps_per_race;
    new->num_teams = num_teams;
    new->max_cars_per_team = max_cars_per_team;

    // TODO construct fields validations.

    return new;
}

char * race_config_to_string(race_config_t * cfg) {
    char * buffer = NULL;

    if (cfg == NULL) {
        buffer = string(NULL_STR_SIZE);

        snprintf(buffer, NULL_STR_SIZE * sizeof(char), "NULL");
    } else {
        buffer = string(BUF_SIZE);

        snprintf(buffer, BUF_SIZE * sizeof(char), "\nTime units per second: %.2ftu\n"
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
    }

    return buffer;
}

// endregion public functions