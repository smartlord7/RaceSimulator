#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_handler.h"
#include "read_line.h"
#include "to_float.h"

#define MAX_CONFIG_FILE_LINE_SIZE 20
#define CONFIG_FILE_NUM_LINES 7
#define MIN_NUM_TEAMS 3
#define FIELD_DELIMITER ","


typedef struct race_config race_config;

struct race_config {
    float time_units_per_sec,
           lap_distance,
           laps_per_race,
           malfunction_interval,
           repair_min_time,
           repair_max_time,
           fuel_tank_capacity;
    int num_teams, cars_per_team;
};

void to_float_wrapper(int feedback, int line, const char * file_name, const char * field_name) {
    if (feedback == FLOAT_SIZE_EXCEEDED) {
        throw_error(strcat(ERROR_AT_LINE, ERROR_FLOAT_SIZE_EXCEEDED), line, file_name, field_name);
    } else if (feedback == FLOAT_CONVERSION_FAILURE) {
        throw_error(strcat(ERROR_AT_LINE, ERROR_FLOAT_CONVERSION), line, file_name, field_name);
    }
}

race_config * read_race_config(const char * config_file_path) {
    FILE * config_file = NULL;
    race_config * config = NULL;

    if ((config_file = fopen(config_file_path, "r")) == NULL) {
        throw_error(ERROR_FILE_OPENING, config_file_path);
    }

    if ((config = malloc(sizeof(race_config))) == NULL) {
        throw_error(ERROR_MEMORY_ALLOCATION, "RACE CONFIG");
    }

    char buffer[MAX_CONFIG_FILE_LINE_SIZE], * token = NULL;
    float time_units_per_sec,
            lap_distance,
            laps_per_race,
            malfunction_interval,
            repair_min_time,
            repair_max_time,
            fuel_tank_capacity;
    int num_teams, cars_per_team, i = 0;

   while (i < CONFIG_FILE_NUM_LINES) {
       int read_feedback = read_line(buffer, config_file, MAX_CONFIG_FILE_LINE_SIZE);

       if (read_feedback == EOF) {
           throw_error(ERROR_INSUFFICIENT_NUM_LINES, config_file_path, CONFIG_FILE_NUM_LINES);
       } else if (read_feedback == BUFFER_SIZE_EXCEEDED) {
           throw_error(strcat(ERROR_AT_LINE, ERROR_BUFFER_SIZE_EXCEEDED), i + 1, config_file_path, CONFIG_FILE_NUM_LINES);
       }

       switch(i) {
           case 0:
               to_float_wrapper(to_float(buffer, &time_units_per_sec), i + 1, config_file_path, "time_units_per_sec");

               if (time_units_per_sec <= 0) {
                   throw_error(strcat(ERROR_AT_LINE, ERROR_MIN_VALUE_VIOLATED), i + 1, config_file_path, "time_units_per_sec", 0);
               }

               config->time_units_per_sec = time_units_per_sec;

               break;
           case 1:
               if ((token = strtok(buffer, FIELD_DELIMITER)) == NULL) {
                   throw_error(strcat(ERROR_AT_LINE, ERROR_STRTOK), i + 1, config_file_path, "lap_distance");
               }

               to_float_wrapper(to_float(token, &lap_distance), i + 1, config_file_path, "lap_distance");

               if (lap_distance <= 0) {
                   throw_error(strcat(ERROR_AT_LINE, ERROR_MIN_VALUE_VIOLATED), i + 1, config_file_path, "lap_distance", 0);
               }

               if ((token = strtok(buffer, FIELD_DELIMITER)) == NULL) {
                   throw_error(strcat(ERROR_AT_LINE, ERROR_STRTOK), i + 1, config_file_path, "laps_per_race");
               }

               if (!(laps_per_race = atoi(token))) {
                   throw_error(strcat(ERROR_AT_LINE, ERROR_INT_CONVERSION), i + 1, config_file_path, "laps_per_race");
               }

               if (laps_per_race <= 0) {
                   throw_error(strcat(ERROR_AT_LINE, ERROR_MIN_VALUE_VIOLATED), i + 1, config_file_path, "laps_per_race", 0);
               }

               config->lap_distance = lap_distance;
               config->laps_per_race = laps_per_race;

               break;
           case 2:
               if (!(num_teams = atoi(buffer))) {
                   throw_error(strcat(ERROR_AT_LINE, ERROR_INT_CONVERSION), i + 1, config_file_path, "num_teams");
               }

               if (num_teams < MIN_NUM_TEAMS) {
                   throw_error(strcat(ERROR_AT_LINE, ERROR_MIN_VALUE_VIOLATED), i + 1, config_file_path, "num_teams", MIN_NUM_TEAMS);
               }

               config->num_teams = num_teams;

               break;
           case 3:
               if (!(cars_per_team = atoi(buffer))) {
                   throw_error(strcat(ERROR_AT_LINE, ERROR_INT_CONVERSION), i + 1, config_file_path, "cars_per_team");
               }

               if (cars_per_team < 0) {
                   throw_error(strcat(ERROR_AT_LINE, ERROR_MIN_VALUE_VIOLATED), i + 1, config_file_path, "num_teams", MIN_NUM_TEAMS);
               }

               config->num_teams = num_teams;

               break;
           case 4:
               break;
           case 5:
               break;
           case 6:
               break;
       }

       i++;
   }


    return config;
}
