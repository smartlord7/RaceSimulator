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
#include <string.h>
#include <assert.h>
#include <time.h>
#include <semaphore.h>
#include "../strings/strings.h"
#include "../exception_handler/exception_handler.h"
#include "../../ipcs/sync/semaphore/sem.h"
#include "log_generator.h"

// endregion dependencies

// region private functions prototypes

/**
 * @def write_log_entry
 * @brief Function that writes a given entry in the log file.
 *
 * @param entry
 * String to be written in the log file.
 */
void write_log_entry(char * entry);

/**
 * @def get_time
 * @brief Function that gives the current time in the hh:mm:ss format.
 *
 * @return Current local system time in the hh:mm:ss format.
 */
char * get_time();

// endregion private functions prototypes

// region global variables

const char * log_file_path = NULL;
FILE * log_file;

// endregion global variables

// region public functions

void log_init(const char * lg_file_path) {
    if ((log_file = fopen(lg_file_path, "a")) == NULL){
        throw_and_exit(FILE_OPEN_EXCEPTION, lg_file_path);
    }

    log_file_path = lg_file_path;
}

void log_close(){

    if (fclose(log_file)){
        throw_and_exit(FILE_CLOSE_EXCEPTION, log_file_path);
    }

}

void generate_log_entry(char * mode, void * data){
    assert(mode != NULL);

    //race_car_t * car;
    char * entry = get_time();

    if (strcmp(mode, I_SIMULATION_START) == 0){
        entry = append(entry, O_SIMULATION_STARTING);

    } else if (strcmp(mode, I_SIMULATION_END) == 0){
        entry = append(entry, O_SIMULATION_CLOSING);

    } else if (strcmp(mode, I_COMMAND_RECEIVED) == 0){
        entry = append(entry, O_NEW_COMMAND_RECEIVED);
        entry = append(entry, (char *) data);

    } else if(strcmp(mode, I_COMMAND_EXCEPTION) == 0){
        entry = append(entry, O_WRONG_COMMAND);
        entry = append(entry, (char *) data);

    } else if(strcmp(mode, I_CAR_LOADED) == 0){
        //car = (race_car_t *) data;
        entry = append(entry,O_CAR_LOADED);
        entry = append(entry, O_TEMP_NUM);

    } else if(strcmp(mode, I_CAR_MALFUNCTION) == 0){
        //car = (race_car_t *) data;
        entry = append(entry, O_NEW_CAR_PROBLEM);
        entry = append(entry, O_TEMP_NUM);

    } else if(strcmp(mode, I_SIGNAL_RECEIVED) == 0){
        entry = append(entry, O_SIGNAL);
        entry = append(entry, (char *) data);
        entry = append(entry, O_RECEIVED);

    } else if(strcmp(mode, I_RACE_WIN) == 0){
        //car = (race_car_t *) data;
        entry = append(entry, O_CAR);
        entry = append(entry, O_TEMP_NUM);
        entry = append(entry, O_RACE_WON);

    } else throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);

    printf("\n%s\n", entry);
    write_log_entry(entry);
}

// endregion public functions

// region private functions

void write_log_entry(char * entry){
    assert(entry != NULL);

    if (fprintf(log_file, "%s\n", entry) < 0){
        throw_and_exit(FILE_WRITE_EXCEPTION, log_file_path);
    }
}

char * get_time(){
    time_t current_time;
    char * buffer, * token, * result;
    int i;

    current_time = time(NULL);
    buffer = asctime(localtime(&current_time));

    if ((token = strtok(buffer, TIMESTAMP_DELIMITER)) == NULL) {
        throw_and_exit(TOKENIZING_EXCEPTION, OF_TIMESTAMP);
    }

    for(i = 0; i < NUM_TIMESTAMP_FIELDS; i++){
        if ((token = strtok(NULL, TIMESTAMP_DELIMITER)) == NULL) {
            throw_and_exit(TOKENIZING_EXCEPTION, OF_TIMESTAMP);
        }
    }

    result = token;

    return result;
}

// endregion private functions