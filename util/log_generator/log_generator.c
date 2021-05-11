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
#include "../global.h"

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

void generate_log_entry(int mode, void * data){

    race_car_t * car;
    char entry[LARGEST_SIZE];

    HERE("1");

    switch (mode) {
        case I_SIMULATION_START:
            HERE("2");
            snprintf(entry, LARGEST_SIZE, "%s => STARTING SIMULATION\n", get_time());
            break;
        case I_SIMULATION_END:
            HERE("3");
            snprintf(entry, LARGEST_SIZE, "%s => CLOSING SIMULATION\n", get_time());
            break;
        case I_COMMAND_RECEIVED:
            HERE("4");
            snprintf(entry, LARGEST_SIZE, "%s => COMMAND RECEIVED: %s\n", get_time(), (char *) data);
            break;
        case I_COMMAND_EXCEPTION:
            HERE("5");
            snprintf(entry, LARGEST_SIZE, "%s => WRONG COMMAND: %s\n", get_time(), (char *) data);
            break;
        case I_CAR_LOADED:
            HERE("6");
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %d FROM TEAM %d LOADED\n", get_time(), car->car_id,
                     car->team->team_id);
            break;
        case I_CAR_REJECTED:
            HERE("7");
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %d FROM TEAM %d REJECTED\n", get_time(), car->car_id,
                     car->team->team_id);
            break;
        case I_RACE_START:
            HERE("8");
            snprintf(entry, LARGEST_SIZE, "%s => RACE STARTED!\n", get_time());
            break;
        case I_CANNOT_START:
            HERE("9");
            snprintf(entry, LARGEST_SIZE, "%s => RACE CANNOT START!\n", get_time());
            break;
        case I_CAR_MALFUNCTION:
            HERE("10");
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %d FROM TEAM %d SUFFERED MALFUNCTION\n", get_time(), car->car_id,
                     car->team->team_id);
            break;
        case I_SIGNAL_RECEIVED:
            HERE("11");
            snprintf(entry, LARGEST_SIZE, "%s => SIGNAL %s RECEIVED!\n", get_time(), (char *) data);
            break;
        case I_RACE_WIN:
            HERE("12");
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %d FROM TEAM %d WON!\n", get_time(), car->car_id,
                     car->team->team_id);
        default:
            throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);
            break;
    }

    printf("%s", entry);
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
        throw_and_exit(TOKENIZE_EXCEPTION, OF_TIMESTAMP);
    }

    for(i = 0; i < NUM_TIMESTAMP_FIELDS; i++){
        if ((token = strtok(NULL, TIMESTAMP_DELIMITER)) == NULL) {
            throw_and_exit(TOKENIZE_EXCEPTION, OF_TIMESTAMP);
        }
    }

    result = token;

    return result;
}

// endregion private functions