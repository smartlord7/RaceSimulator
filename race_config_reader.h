/*
 * Authors:
 *  - Joao Filipe Guiomar Artur, 2019217853
 *  - Sancho Amaral Simoes, 2019217590
 *
 * Date of creation: 02/04/2021
 */

#ifndef RACESIMULATOR_RACE_CONFIG_READER_H
#define RACESIMULATOR_RACE_CONFIG_READER_H

#include "structs/race_config_t.h"

#define true 1
#define false 0

#define MAX_CONFIG_FILE_LINE_SIZE 20
#define CONFIG_FILE_NUM_LINES 7
#define FIELD_DELIMITER ","
#define MAX_BUFFER_SIZE 512

#define MIN_TIME_UNITS_PER_SEC 1
#define MIN_LAP_DISTANCE 1000
#define MIN_LAPS_PER_RACE 1
#define MIN_NUM_TEAMS 3
#define MIN_MAX_CARS_PER_TEAM 1
#define MIN_MALFUNCTION_TIME 10
#define MIN_MIN_REPAIR_TIME 10
#define MIN_MAX_REPAIR_TIME 30
#define MIN_FUEL_TANK_CAPACITY 40
#define MAX_TIME_UNITS_PER_SEC 60
#define MAX_LAP_DISTANCE 50000
#define MAX_LAPS_PER_RACE 25
#define MAX_NUM_TEAMS 15
#define MAX_MAX_CARS_PER_TEAM 10
#define MAX_MALFUNCTION_TIME 300
#define MAX_MIN_REPAIR_TIME 60
#define MAX_MAX_REPAIR_TIME 180
#define MAX_FUEL_TANK_CAPACITY 100

#define RACE_CONFIG "race_config"
#define TIME_UNITS_PER_SEC "time_units_per_sec"
#define LAP_DISTANCE "lap_distance"
#define LAPS_PER_RACE "laps_per_race"
#define NUM_TEAMS "num_teams"
#define MAX_CARS_PER_TEAM "max_cars_per_team"
#define MALFUNCTION_INTERVAL "malfunction_interval"
#define MIN_REPAIR_TIME "min_repair_time"
#define MAX_REPAIR_TIME "max_repair_time"
#define FUEL_TANK_CAPACITY "fuel_tank_capacity"

/**
 * Initialize configuration file reading functionality.
 * @param cfg_file_path Path of configuration file.
 */
void race_config_reader_init(char * cfg_file_path);

/**
 * Read the configuration file.
 * @return Pointer to config memory space.
 */
race_config_t * read_race_config();

#endif //RACESIMULATOR_RACE_CONFIG_READER_H