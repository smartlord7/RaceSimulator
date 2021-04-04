/* Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* Authors:
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* Date of creation: 31/03/2021
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include "util/exception_handler.h"
#include "util/ipc_manager.h"
#include "log_generator.h"

#define TIMESTAMP_DELIMITER " "
#define MUTEX "MUTEX"
#define true 1
#define false 0

/**
 * Write an entry on log file.
 * @param entry C String to be written on log file.
 */
void write_log_entry(char * entry);

/**
 * Get the local system time.
 * @return Current local system time in hh:mm:ss format.
 */
char * get_time();

/**
 * Concatenate two given strings.
 * @param stringA String to be the first part and to store the result.
 * @param stringB String to be concatenated.
 * @param exception_context Context in which the exception might have occurred.
 * @return Return Result of the concatenated strings.
 */
char * str_concat(char * stringA, char * stringB, char * exception_context);

/** Variables */
static char * log_file_path = NULL;
static sem_t * mutex = NULL;

void log_init(char * lg_file_path, sem_t * sem) {
    log_file_path = lg_file_path;
    mutex = sem;
}

void generate_log_entry(char * mode, void * data){
    //race_car_t * car;
    char * entry = get_time();

    if (strcmp(mode, I_SIMULATION_START) == 0){
        entry = str_concat(entry, O_SIMULATION_STARTING, I_SIMULATION_START);

    } else if (strcmp(mode, I_SIMULATION_END) == 0){
        entry = str_concat(entry, O_SIMULATION_CLOSING, I_SIMULATION_END);

    } else if (strcmp(mode, I_COMMAND_RECEIVED) == 0){
        entry = str_concat(entry, O_NEW_COMMAND_RECEIVED, I_COMMAND_RECEIVED);
        entry = str_concat(entry, (char *) data, I_COMMAND_RECEIVED);

    } else if(strcmp(mode, I_COMMAND_EXCEPTION) == 0){
        entry = str_concat(entry, O_WRONG_COMMAND, I_COMMAND_EXCEPTION);
        entry = str_concat(entry, (char *) data, I_COMMAND_EXCEPTION);

    } else if(strcmp(mode, I_CAR_LOADED) == 0){
        //car = (race_car_t *) data;
        entry = str_concat(entry,O_CAR_LOADED, I_CAR_LOADED);
        entry = str_concat(entry, O_TEMP_NUM, I_CAR_LOADED);

    } else if(strcmp(mode, I_CAR_MALFUNCTION) == 0){
        //car = (race_car_t *) data;
        entry = str_concat(entry, O_NEW_CAR_PROBLEM, I_CAR_MALFUNCTION);
        entry = str_concat(entry, O_TEMP_NUM, I_CAR_MALFUNCTION);

    } else if(strcmp(mode, I_SIGNAL_RECEIVED) == 0){
        entry = str_concat(entry, O_SIGNAL, I_SIGNAL_RECEIVED);
        entry = str_concat(entry, (char *) data, I_SIGNAL_RECEIVED);
        entry = str_concat(entry, O_RECEIVED, I_SIGNAL_RECEIVED);

    } else if(strcmp(mode, I_RACE_WIN) == 0){
        //car = (race_car_t *) data;
        entry = str_concat(entry, O_CAR, I_RACE_WIN);
        entry = str_concat(entry, O_TEMP_NUM, I_RACE_WIN);
        entry = str_concat(entry, O_RACE_WON, I_RACE_WIN);

    } else throw_exception_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);

    entry = str_concat(entry, O_TERMINATOR, NULL);

    wait_sem(mutex, MUTEX);

    printf("\n%s\n", entry);
    write_log_entry(entry);

    post_sem(mutex, MUTEX);
}

void write_log_entry(char * entry){
    FILE * log_file;

    if ((log_file = fopen(log_file_path, "a")) == NULL){
        throw_exception_and_stay(FILE_OPENING_EXCEPTION, log_file_path);
    }

    if (fprintf(log_file, "%s\n", entry) < 0){
        throw_exception_and_stay(FILE_WRITING_EXCEPTION, log_file_path);
    }

    if (fclose(log_file)){
        throw_exception_and_stay(FILE_CLOSING_EXCEPTION, log_file_path);
    }

}

char * str_concat(char * stringA, char * stringB, char * exception_context) {

    if (strcat(stringA, stringB) == NULL) {
        throw_exception_and_stay(LOG_ENTRY_GENERATION_EXCEPTION, exception_context);
    }

    return stringA;
}

char * get_time(){
    time_t current_time;
    char * buffer, * token, * result;
    int i;

    current_time = time(NULL);
    buffer = asctime(localtime(&current_time));

    if ((token = strtok(buffer, TIMESTAMP_DELIMITER)) == NULL) {
        throw_exception_and_stay(TOKENIZING_EXCEPTION, "OF TIMESTAMP");
    }

    for(i = 0; i < 3; i++){
        if ((token = strtok(NULL, TIMESTAMP_DELIMITER)) == NULL) {
            throw_exception_and_stay(TOKENIZING_EXCEPTION, "OF TIMESTAMP");
        }
    }

    result = token;
    return result;
}