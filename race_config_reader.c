#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/error_handler.h"
#include "util/read_line.h"
#include "util/to_float.h"
#include "structs/race_config.h"
#include <stdarg.h>

#define true 1
#define false 0

#define MAX_CONFIG_FILE_LINE_SIZE 20
#define CONFIG_FILE_NUM_LINES 7
#define MIN_NUM_TEAMS 3
#define FIELD_DELIMITER ","
#define MIN_VALUE 0

#define MAX_TIME_UNIT_PER_SEC 60
#define MAX_LAP_DISTANCE 50000
#define MAX_LAPS_PER_RACE 25
#define MAX_NUM_TEAMS 15
#define MAX_CARS_PER_TEAM 10
#define MAX_MALFUNCT_TIME 300
#define MAX_T_BOX_MIN 60
#define MAX_T_BOX_MAX 180
#define MAX_DEPOSIT_CAPACITY 100

static char * config_file_path = NULL;
static int current_line;

void race_config_reader_init(char * cfg_file_path) {
    config_file_path = cfg_file_path;
    current_line = -1;
}

void race_config_reader_reset() {
    config_file_path = NULL;
    current_line = -1;
}

void show_race_config(race_config * cfg) {
    printf("Time units per second: %.2ftu\n"
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

static void error_at_line(const char * error_msg, ...) {
    va_list args;
    va_start(args, error_msg);

    char buffer[MAX_ERROR_MSG_SIZE];
    vsnprintf(buffer, sizeof(buffer), error_msg, args);
    char buffer2[MAX_ERROR_MSG_SIZE * 2];

    snprintf(buffer2, sizeof(buffer2), "%s %s", ERROR_AT_LINE, buffer);
    throw_error_end_exit(buffer2, current_line + 1, config_file_path);

    va_end(args);
}

static void to_float_wrapper(int feedback, const char * field_name) {
    if (feedback == FLOAT_SIZE_EXCEEDED) {
        error_at_line(ERROR_FLOAT_SIZE_EXCEEDED, field_name);
    } else if (feedback == FLOAT_CONVERSION_FAILURE) {
        error_at_line(ERROR_FLOAT_CONVERSION, field_name);
    }
}

race_config * read_race_config() {
    if (config_file_path == NULL) {
        return NULL;
    }

    FILE * config_file = NULL;
    race_config * config = NULL;

    if ((config_file = fopen(config_file_path, "r")) == NULL) {
        throw_error_end_exit(ERROR_FILE_OPENING, config_file_path);
    }

    if ((config = malloc(sizeof(race_config))) == NULL) {
        throw_error_end_exit(ERROR_MEMORY_ALLOCATION, "race config object");
    }

    current_line = 0;

    char buffer[MAX_CONFIG_FILE_LINE_SIZE], * token = NULL;
    float time_units_per_sec,
            lap_distance,
            malfunction_interval,
            repair_min_time,
            repair_max_time,
            fuel_tank_capacity;
    int num_teams, laps_per_race, cars_per_team;

   while (current_line < CONFIG_FILE_NUM_LINES) {
       int read_feedback = read_line(buffer, config_file, MAX_CONFIG_FILE_LINE_SIZE);

       if (read_feedback == EOF && buffer[0] == '\0') {
           throw_error_end_exit(ERROR_INSUFFICIENT_NUM_LINES, config_file_path, CONFIG_FILE_NUM_LINES);
       } else if (read_feedback == BUFFER_SIZE_EXCEEDED) {
           error_at_line(ERROR_BUFFER_SIZE_EXCEEDED, CONFIG_FILE_NUM_LINES);
       }

       switch(current_line) {
           case 0:
               to_float_wrapper(to_float(buffer, &time_units_per_sec), "time_units_per_sec");

               if (time_units_per_sec <= MIN_VALUE) {
                   error_at_line(ERROR_MIN_VALUE_VIOLATED, "time_units_per_sec", MIN_VALUE);
               } else if (time_units_per_sec > MAX_TIME_UNIT_PER_SEC) {
                   error_at_line(ERROR_MAX_VALUE_VIOLATED, "time_units_per_sec", MAX_TIME_UNIT_PER_SEC);
               }

               config->time_units_per_sec = time_units_per_sec;

               break;
           case 1:
               if ((token = strtok(buffer, FIELD_DELIMITER)) == NULL) {
                   error_at_line(ERROR_STRTOK, "lap_distance");
               }

               to_float_wrapper(to_float(token, &lap_distance), "lap_distance");

               if (lap_distance <= MIN_VALUE) {
                   error_at_line(ERROR_MIN_VALUE_VIOLATED, "lap_distance", MIN_VALUE);
               } else if (lap_distance > MAX_LAP_DISTANCE) {
                   error_at_line(ERROR_MAX_VALUE_VIOLATED, "lap_distance", MAX_LAP_DISTANCE);
               }

               if ((token = strtok(NULL, FIELD_DELIMITER)) == NULL) {
                   error_at_line(ERROR_STRTOK, "laps_per_race");
               }

               if (!(laps_per_race = atoi(token))) {
                   error_at_line(ERROR_INT_CONVERSION, "laps_per_race");
               }

               if (laps_per_race <= MIN_VALUE) {
                   error_at_line(ERROR_MIN_VALUE_VIOLATED, "laps_per_race", MIN_VALUE);
               } else if (laps_per_race > MAX_LAPS_PER_RACE) {
                   error_at_line(ERROR_MAX_VALUE_VIOLATED, "laps_per_race", MAX_LAPS_PER_RACE);
               }

               config->lap_distance = lap_distance;
               config->laps_per_race = laps_per_race;

               break;
           case 2:
               if (!(num_teams = atoi(buffer))) {
                   error_at_line(ERROR_INT_CONVERSION, "num_teams");
               }

               if (num_teams < MIN_NUM_TEAMS) {
                   error_at_line(ERROR_MIN_VALUE_VIOLATED, "num_teams", MIN_NUM_TEAMS);
               } else if (num_teams > MAX_NUM_TEAMS){
                   error_at_line(ERROR_MAX_VALUE_VIOLATED, "num_teams", MAX_NUM_TEAMS);
               }

               config->num_teams = num_teams;

               break;
           case 3:
               if (!(cars_per_team = atoi(buffer))) {
                   error_at_line(ERROR_INT_CONVERSION, "cars_per_team");
               }

               if (cars_per_team < MIN_VALUE) {
                   error_at_line(ERROR_MIN_VALUE_VIOLATED, "cars_per_team", MIN_NUM_TEAMS);
               } else if (cars_per_team > MAX_CARS_PER_TEAM){
                   error_at_line(ERROR_MAX_VALUE_VIOLATED, "cars_per_team", MAX_CARS_PER_TEAM);
               }

               config->max_cars_per_team = cars_per_team;

               break;
           case 4:
               to_float_wrapper(to_float(buffer, &malfunction_interval), "malfunction_interval");

               if (malfunction_interval <= MIN_VALUE) {
                   error_at_line(ERROR_MIN_VALUE_VIOLATED, "malfunction_interval", MIN_VALUE);
               } else if (malfunction_interval > MAX_MALFUNCT_TIME) {
                   error_at_line(ERROR_MAX_VALUE_VIOLATED, "malfunction_interval", MAX_MALFUNCT_TIME);
               }

               config->malfunction_interval = malfunction_interval;

               break;
           case 5:
               if ((token = strtok(buffer, FIELD_DELIMITER)) == NULL) {
                   error_at_line(ERROR_STRTOK, "min_repair_time");
               }

               if (!(repair_min_time = atoi(token))) {
                   error_at_line(ERROR_INT_CONVERSION, "min_repair_time");
               }

               if (repair_min_time <= MIN_VALUE) {
                   error_at_line(ERROR_MIN_VALUE_VIOLATED, "min_repair_time", MIN_VALUE);
               } else if (repair_min_time > MAX_T_BOX_MIN){
                   error_at_line(ERROR_MAX_VALUE_VIOLATED, "min_repair_time", MAX_T_BOX_MIN);
               }

               if ((token = strtok(NULL, FIELD_DELIMITER)) == NULL) {
                   error_at_line(ERROR_STRTOK, "max_repair_time");
               }

               if (!(repair_max_time = atoi(token))) {
                   error_at_line(ERROR_INT_CONVERSION, "max_repair_time");
               }

               if (repair_max_time <= MIN_VALUE) {
                   error_at_line(ERROR_MIN_VALUE_VIOLATED, "max_repair_time", MIN_VALUE);
               } else if (repair_max_time > MAX_T_BOX_MAX){
                   error_at_line(ERROR_MAX_VALUE_VIOLATED, "max_repair_time", MAX_T_BOX_MAX);
               }

               if(repair_max_time < repair_min_time){
                   throw_error_end_exit(ERROR_INVALID_TIME_INTERVAL, "max_repair_time", "min_repair_time");
               }

               config->min_repair_time = repair_min_time;
               config->max_repair_time = repair_max_time;

               break;
           case 6:
               to_float_wrapper(to_float(buffer, &fuel_tank_capacity), "fuel_tank_capacity");

               if (fuel_tank_capacity <= MIN_VALUE) {
                   error_at_line(ERROR_MIN_VALUE_VIOLATED, "fuel_tank_capacity", MIN_VALUE);
               } else if (fuel_tank_capacity > MAX_DEPOSIT_CAPACITY){
                   error_at_line(ERROR_MAX_VALUE_VIOLATED, "fuel_tank_capacity", MAX_DEPOSIT_CAPACITY);
               }

               config->fuel_tank_capacity = fuel_tank_capacity;

               break;
       }

       buffer[0] = '\0';

       current_line++;
   }

    return config;
}
