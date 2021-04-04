/*
 * Authors:
 *  - Joao Filipe Guiomar Artur, 2019217853
 *  - Sancho Amaral Simoes, 2019217590
 *
 * Date of creation: 02/04/2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util/exception_handler.h"
#include "util/read_line.h"
#include "util/to_float.h"
#include "structs/race_config_t.h"

#define true 1
#define false 0

//used to read the config file
#define MAX_CONFIG_FILE_LINE_SIZE 20
#define CONFIG_FILE_NUM_LINES 7
#define FIELD_DELIMITER ","
#define MAX_BUFFER_SIZE 512

//used to define an internal interval to each of the various types of value
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

//used to define the meaning of the different values
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
 * Shows the line and the file name where a parsing error occurred.
 * @param error_msg Formatted error message.
 * @param ... Formatting arguments.
 */
static void error_at_line(const char * error_msg, ...);

/**
* Check the nature of the feedback after attempting to convert a string to float.
* @param feedback
* @param field_name Meaning of the value.
*/
static void to_float_wrapper(int feedback, const char * field_name);

/**
 * Check if a given value belongs to a given interval for acceptable values.
 * @param value Value to be validated.
 * @param field_name Meaning of the value.
 * @param min Minimum value of the interval.
 * @param max Maximum value of the interval.
 */
void validate_interval(float value, const char * field_name, float min, float max);

/** Variables */
static char * config_file_path = NULL;
static int current_line;

void race_config_reader_init(char * cfg_file_path) {
    config_file_path = cfg_file_path;
    current_line = -1;
}

race_config_t * read_race_config() {
    if (config_file_path == NULL) {
        return NULL;
    }

    FILE * config_file = NULL;
    race_config_t * config = NULL;

    if ((config_file = fopen(config_file_path, "r")) == NULL) {
        throw_exception_and_exit(FILE_OPENING_EXCEPTION, config_file_path);
    }

    if ((config = malloc(sizeof(race_config_t))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, RACE_CONFIG);
    }

    current_line = 0;

    char buffer[MAX_CONFIG_FILE_LINE_SIZE], * token = NULL;
    float time_units_per_sec,
            lap_distance,
            malfunction_interval,
            repair_min_time,
            repair_max_time,
            fuel_tank_capacity;
    int num_teams, laps_per_race, max_cars_per_team;

   while (current_line < CONFIG_FILE_NUM_LINES) {
       int read_feedback = read_line(buffer, config_file, MAX_CONFIG_FILE_LINE_SIZE);

       if (read_feedback == EOF && buffer[0] == '\0') {
           throw_exception_and_exit(INSUFFICIENT_NUM_LINES_EXCEPTION, config_file_path, CONFIG_FILE_NUM_LINES);
       } else if (read_feedback == BUFFER_SIZE_EXCEEDED) {
           error_at_line(BUFFER_SIZE_EXCEEDED_EXCEPTION, MAX_CONFIG_FILE_LINE_SIZE);
       }

       switch(current_line) {
           case 0:
               to_float_wrapper(to_float(buffer, &time_units_per_sec), TIME_UNITS_PER_SEC);

               validate_interval(time_units_per_sec, TIME_UNITS_PER_SEC, MIN_TIME_UNITS_PER_SEC, MAX_TIME_UNITS_PER_SEC);

               config->time_units_per_sec = time_units_per_sec;

               break;
           case 1:
               if ((token = strtok(buffer, FIELD_DELIMITER)) == NULL) {
                   error_at_line(TOKENIZING_EXCEPTION, LAP_DISTANCE);
               }

               to_float_wrapper(to_float(token, &lap_distance), LAP_DISTANCE);

               validate_interval(lap_distance, LAP_DISTANCE, MIN_LAP_DISTANCE, MAX_LAP_DISTANCE);

               if ((token = strtok(NULL, FIELD_DELIMITER)) == NULL) {
                   error_at_line(TOKENIZING_EXCEPTION, LAPS_PER_RACE);
               }

               if (!(laps_per_race = atoi(token))) {
                   error_at_line(INT_CONVERSION_EXCEPTION, LAPS_PER_RACE);
               }

               validate_interval((float) laps_per_race, LAPS_PER_RACE, MIN_LAPS_PER_RACE, MAX_LAPS_PER_RACE);

               config->lap_distance = lap_distance;
               config->laps_per_race = laps_per_race;

               break;
           case 2:
               if (!(num_teams = atoi(buffer))) {
                   error_at_line(INT_CONVERSION_EXCEPTION, NUM_TEAMS);
               }

               validate_interval((float) num_teams, NUM_TEAMS, MIN_NUM_TEAMS, MAX_NUM_TEAMS);

               config->num_teams = num_teams;

               break;
           case 3:
               if (!(max_cars_per_team = atoi(buffer))) {
                   error_at_line(INT_CONVERSION_EXCEPTION, MAX_CARS_PER_TEAM);
               }

               validate_interval((float) max_cars_per_team, MAX_CARS_PER_TEAM, MIN_MAX_CARS_PER_TEAM, MAX_MAX_CARS_PER_TEAM);

               config->max_cars_per_team = max_cars_per_team;

               break;
           case 4:
               to_float_wrapper(to_float(buffer, &malfunction_interval), MALFUNCTION_INTERVAL);

               validate_interval(malfunction_interval, MALFUNCTION_INTERVAL, MIN_MALFUNCTION_TIME, MAX_MALFUNCTION_TIME);

               config->malfunction_interval = malfunction_interval;

               break;
           case 5:
               if ((token = strtok(buffer, FIELD_DELIMITER)) == NULL) {
                   error_at_line(TOKENIZING_EXCEPTION, MIN_REPAIR_TIME);
               }

               if (!(repair_min_time = atoi(token))) {
                   error_at_line(INT_CONVERSION_EXCEPTION, MIN_REPAIR_TIME);
               }

               validate_interval(repair_min_time, MIN_REPAIR_TIME, MIN_MIN_REPAIR_TIME, MAX_MIN_REPAIR_TIME);

               if ((token = strtok(NULL, FIELD_DELIMITER)) == NULL) {
                   error_at_line(TOKENIZING_EXCEPTION, MAX_REPAIR_TIME);
               }

               if (!(repair_max_time = atoi(token))) {
                   error_at_line(INT_CONVERSION_EXCEPTION, MAX_REPAIR_TIME);
               }

               validate_interval(repair_max_time, MAX_REPAIR_TIME, MIN_MAX_REPAIR_TIME, MAX_MAX_REPAIR_TIME);

               validate_interval(repair_min_time, MIN_REPAIR_TIME, repair_min_time, repair_max_time);

               config->min_repair_time = repair_min_time;
               config->max_repair_time = repair_max_time;

               break;
           case 6:
               to_float_wrapper(to_float(buffer, &fuel_tank_capacity), FUEL_TANK_CAPACITY);

               validate_interval(fuel_tank_capacity, FUEL_TANK_CAPACITY, MIN_FUEL_TANK_CAPACITY, MAX_FUEL_TANK_CAPACITY);

               config->fuel_tank_capacity = fuel_tank_capacity;

               break;
       }

       current_line++;
       buffer[0] = '\0';
   }

    if (fclose(config_file) != 0) {
        throw_exception_and_exit(FILE_CLOSING_EXCEPTION, config_file_path);
    }

    return config;
}

static void error_at_line(const char * error_msg, ...) {
    va_list args;
    va_start(args, error_msg);

    char buffer[MAX_EXCEPTION_MSG_SIZE];
    vsnprintf(buffer, sizeof(buffer), error_msg, args);
    char buffer2[MAX_EXCEPTION_MSG_SIZE * 2];

    va_end(args);

    snprintf(buffer2, sizeof(buffer2), "%s %s", ERROR_AT_LINE, buffer);
    throw_exception_and_exit(buffer2, current_line + 1, config_file_path);

}

static void to_float_wrapper(int feedback, const char * field_name) {
    if (feedback == FLOAT_SIZE_EXCEEDED) {
        error_at_line(FLOAT_SIZE_EXCEEDED_EXCEPTION, field_name);
    } else if (feedback == FLOAT_CONVERSION_FAILURE) {
        error_at_line(FLOAT_CONVERSION_EXCEPTION, field_name);
    }
}

void validate_interval(float value, const char * field_name, float min, float max) {
    if (value < min) {
        error_at_line(MIN_VALUE_EXCEPTION, field_name, min);
    } else if (value > max) {
        error_at_line(MAX_VALUE_EXCEPTION, field_name, max);
    }
}