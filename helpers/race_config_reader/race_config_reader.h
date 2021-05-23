/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

#ifndef RACESIMULATOR_RACE_CONFIG_READER_H
#define RACESIMULATOR_RACE_CONFIG_READER_H

// region dependencies

#include "../../structs/race_config/race_config_t.h"

// endregion dependencies

// region constants

#define true 1
#define false 0
#define FIELD_DELIMITER ","

// region constraints

#define MAX_CONFIG_FILE_LINE_SIZE 20
#define CONFIG_FILE_NUM_LINES 7

#define MIN_TIME_UNITS_PER_SEC 1
#define MAX_TIME_UNITS_PER_SEC 10000 // this constraint valid given the CPU bottleneck. After 10000, no time difference will be detected.
#define MIN_LAP_DISTANCE 500
#define MIN_LAPS_PER_RACE 1
#define MIN_NUM_TEAMS 3
#define MIN_MAX_CARS_PER_TEAM 1
#define MIN_MIN_REPAIR_TIME 1
#define MIN_MAX_REPAIR_TIME 4
#define MIN_FUEL_TANK_CAPACITY 20
#define MAX_LAP_DISTANCE 50000
#define MAX_LAPS_PER_RACE 25
#define MIN_MALFUNCTION_TIME 1
#define MAX_MALFUNCTION_TIME 300
#define MAX_MIN_REPAIR_TIME 60
#define MAX_MAX_REPAIR_TIME 180
#define MAX_FUEL_TANK_CAPACITY 100

// endregion constraints

// region field names

#define TIME_UNITS_PER_SEC "time_units_per_sec"
#define LAP_DISTANCE "lap_distance"
#define LAPS_PER_RACE "laps_per_race"
#define NUM_TEAMS "num_teams"
#define MAX_CARS_PER_TEAM "max_cars_per_team"
#define MALFUNCTION_INTERVAL "malfunction_interval"
#define MIN_REPAIR_TIME "min_repair_time"
#define MAX_REPAIR_TIME "max_repair_time"
#define FUEL_TANK_CAPACITY "fuel_tank_capacity"

// endregion field names

/**
 * @def race_config_reader_init
 * @brief Function that initializes the race config reader.
 *
 * @param cfg_file_path
 * The name of the config file.
 *
 */
void race_config_reader_init(char * cfg_file_path);

/**
 * @def read_race_config
 * @brief Function that reads the race configuration given in a certain file.
 *
 * @return a pointer to the race config.
 */
race_config_t * read_race_config();

#endif //RACESIMULATOR_RACE_CONFIG_READER_H