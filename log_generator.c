#include "log_generator.h"
#include <string.h>
#include "util/error_handler.h"
#include <time.h>
#include "structs/race_car_t.h"
#include "global.h"
#include <stdio.h>

#define TIMESTAMP_DELIMITER " "

static char * log_file_path = NULL;

void log_init(char * lg_file_path) {
    log_file_path = lg_file_path;
}

char * get_time(){
    time_t t;
    int i;
    char * current_time, * token;
    char * buffer = ctime(&t);

    if ((token = strtok(buffer, TIMESTAMP_DELIMITER)) == NULL) {
        throw_error_end_stay(ERROR_STRTOK, "OF TIMESTAMP");
    }
    for(i = 0; i < 3; i++){
        if ((token = strtok(NULL, TIMESTAMP_DELIMITER)) == NULL) {
            throw_error_end_stay(ERROR_STRTOK, "OF TIMESTAMP");
        }
    }
    current_time = token;
    return current_time;
}

void write_log_entry(char * entry);

void generate_log_entry(int mode, void * data){
    race_car_t * car;
    char * entry = get_time();

    switch(mode){
        case SIMULATION_START:
            if(strcat(entry, " SIMULATOR STARTING") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "SIMULATION START");
            }
            break;
        case SIMULATION_END:
            if(strcat(entry, " SIMULATOR CLOSING") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "SIMULATION END");
            }
            break;
        case COMMAND_RECEIVED:
            if(strcat(entry, " NEW COMMAND RECEIVED: ") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "COMMAND RECEIVED");
            }
            if(strcat(entry, (char *) data) == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "COMMAND RECEIVED");
            }
            break;
        case COMMAND_ERROR:
            if(strcat(entry, " WRONG COMMAND => ") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "WRONG COMMAND");
            }
            if(strcat(entry, (char *) data) == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "WRONG COMMAND");
            }
            break;
        case CAR_LOADED:
            car = (race_car_t *) data;
            if(strcat(entry, " CAR LOADED => ") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "CAR LOADED");
            }
            if(strcat(entry, " 0X ") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "WRONG COMMAND");
            }
            break;
        case CAR_MALFUNCTION:
            car = (race_car_t *) data;
            if(strcat(entry, " NEW PROBLEM IN CAR") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "CAR MALFUNCTION");
            }
            if(strcat(entry, " 0X ") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "WRONG COMMAND");
            }
            break;
        case SIGNAL_RECEIVED:
            if(strcat(entry, " SIGNAL") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "SIGNAL RECEIVED");
            }
            if(strcat(entry, (char *) data) == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "SIGNAL RECEIVED");
            }
            if(strcat(entry, " RECEIVED") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "SIGNAL RECEIVED");
            }
            break;
        case RACE_WIN:
            car = (race_car_t *) data;
            if(strcat(entry, " CAR ") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "RACE WIN");
            }
            if(strcat(entry, " 0X ") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "RACE WIN");
            }
            if(strcat(entry, " WINS THE RACE") == NULL){
                throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "RACE WIN");
            }
            break;
    }
    if(strcat(entry, "\0") == NULL){
        throw_error_end_stay(ERROR_GENERATE_LOG_ENTRY, "");
    }

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