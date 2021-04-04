/* Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* Authors:
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* Date of creation: 31/03/2021
*/

#ifndef RACESIMULATOR_RACE_CONFIG_H
#define RACESIMULATOR_RACE_CONFIG_H

typedef struct race_config_t race_config_t;

/**
 * Implement the concept of race configurations.
 */
struct race_config_t {
    float time_units_per_sec,
            lap_distance,
            malfunction_interval,
            min_repair_time,
            max_repair_time,
            fuel_tank_capacity;
    int laps_per_race, num_teams, max_cars_per_team;
};

/**
 * Convert the configurations to strings.
 * @param cfg Configurations' info.
 * @return String with configurations.
 */
char * race_config_to_string(race_config_t * cfg);

#endif //RACESIMULATOR_RACE_CONFIG_H