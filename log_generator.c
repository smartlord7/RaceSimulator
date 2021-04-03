#include "log_generator.h"
#include <string.h>
#include "util/error_handler.h"
#include <time.h>
#include "structs/race_car_t.h"
#include "global.h"
#include <stdio.h>
#include "util/str_concat.h"

#define TIMESTAMP_DELIMITER " "

static char * log_file_path = NULL;

char * get_time();

void write_log_entry(char * entry);

void log_init(char * lg_file_path) {
    log_file_path = lg_file_path;
}

void generate_log_entry(char * mode, void * data){
    race_car_t * car;
    char * entry = get_time();

    if (strcmp(mode, I_SIMULATION_START) == 0){
        entry = str_concat(entry, O_SIMULATION_STARTING, ERROR_GENERATE_LOG_ENTRY, I_SIMULATION_START);

    } else if (strcmp(mode, I_SIMULATION_END) == 0){
        entry = str_concat(entry, O_SIMULATION_CLOSING, ERROR_GENERATE_LOG_ENTRY, I_SIMULATION_END);

    } else if (strcmp(mode, I_COMMAND_RECEIVED) == 0){
        entry = str_concat(entry, O_NEW_COMMAND_RECEIVED, ERROR_GENERATE_LOG_ENTRY, I_COMMAND_RECEIVED);
        entry = str_concat(entry, (char *) data, ERROR_GENERATE_LOG_ENTRY, I_COMMAND_RECEIVED);

    } else if(strcmp(mode, I_COMMAND_ERROR) == 0){
        entry = str_concat(entry, O_WRONG_COMMAND, ERROR_GENERATE_LOG_ENTRY, I_COMMAND_ERROR);
        entry = str_concat(entry, (char *) data, ERROR_GENERATE_LOG_ENTRY, I_COMMAND_ERROR);

    } else if(strcmp(mode, I_CAR_LOADED) == 0){
        car = (race_car_t *) data;
        entry = str_concat(entry,O_CAR_LOADED, ERROR_GENERATE_LOG_ENTRY, I_CAR_LOADED);
        entry = str_concat(entry, O_TEMP_NUM, ERROR_GENERATE_LOG_ENTRY, I_CAR_LOADED);

    } else if(strcmp(mode, I_CAR_MALFUNCTION) == 0){
        car = (race_car_t *) data;
        entry = str_concat(entry, O_NEW_CAR_PROBLEM, ERROR_GENERATE_LOG_ENTRY, I_CAR_MALFUNCTION);
        entry = str_concat(entry, O_TEMP_NUM, ERROR_GENERATE_LOG_ENTRY, I_CAR_MALFUNCTION);

    } else if(strcmp(mode, I_SIGNAL_RECEIVED) == 0){
        entry = str_concat(entry, O_SIGNAL, ERROR_GENERATE_LOG_ENTRY, I_SIGNAL_RECEIVED);
        entry = str_concat(entry, (char *) data, ERROR_GENERATE_LOG_ENTRY, I_SIGNAL_RECEIVED);
        entry = str_concat(entry, O_RECEIVED, ERROR_GENERATE_LOG_ENTRY, I_SIGNAL_RECEIVED);

    } else if(strcmp(mode, I_RACE_WIN) == 0){
        car = (race_car_t *) data;
        entry = str_concat(entry, O_CAR, ERROR_GENERATE_LOG_ENTRY, I_RACE_WIN);
        entry = str_concat(entry, O_TEMP_NUM, ERROR_GENERATE_LOG_ENTRY, I_RACE_WIN);
        entry = str_concat(entry, O_RACE_WON, ERROR_GENERATE_LOG_ENTRY, I_RACE_WIN);

    } else throw_error_end_stay(ERROR_LOG_ENTRY_NOT_SUPPORTED, mode);

    entry = str_concat(entry, O_TERMINATOR, ERROR_GENERATE_LOG_ENTRY, NULL);

    //sem_wait(mutex);

    printf("\n%s\n", entry);
    write_log_entry(entry);

    //sem_post(mutex);
}

void write_log_entry(char * entry){
    FILE * log_file;

    if ((log_file = fopen(log_file_path, "a")) == NULL){
        throw_error_end_stay(ERROR_FILE_OPENING, log_file_path);
    }
    
    if (fprintf(log_file, "%s\n", entry) < 0){
        throw_error_end_stay(ERROR_FILE_WRITE, log_file_path);
    }
    
    if (fclose(log_file)){
        throw_error_end_stay(ERROR_FILE_CLOSING, log_file_path);
    }
    
}

char * get_time(){
    time_t current_time;
    char * buffer, * token, * result;
    int i;

    current_time = time(NULL);
    buffer = asctime(localtime(&current_time));

    if ((token = strtok(buffer, TIMESTAMP_DELIMITER)) == NULL) {
        throw_error_end_stay(ERROR_STRTOK, "OF TIMESTAMP");
    }
    for(i = 0; i < 3; i++){
        if ((token = strtok(NULL, TIMESTAMP_DELIMITER)) == NULL) {
            throw_error_end_stay(ERROR_STRTOK, "OF TIMESTAMP");
        }
    }
    result = token;
    return result;
}