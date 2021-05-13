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
#include "../../util/exception_handler/exception_handler.h"
#include "log_generator.h"
#include "../../util/global.h"

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

    switch (mode) {
        case I_SIMULATION_START:
            snprintf(entry, LARGEST_SIZE, "%s => STARTING SIMULATION", get_time());
            break;
        case I_SIMULATION_END:
            snprintf(entry, LARGEST_SIZE, "%s => CLOSING SIMULATION", get_time());
            break;
        case I_COMMAND_RECEIVED:
            snprintf(entry, LARGEST_SIZE, "%s => COMMAND RECEIVED:\n'%s'", get_time(), (char *) data);
            break;
        case I_COMMAND_REJECTED:
            snprintf(entry, LARGEST_SIZE, "%s => COMMAND REJECTED:\n'%s'", get_time(), (char *) data);
            break;
        case I_CAR_LOADED:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s FROM TEAM '%s' LOADED", get_time(), car->name,
                     car->team->team_name);
            break;
        case I_CAR_REJECTED:
            snprintf(entry, LARGEST_SIZE, "%s => CAR REJECTED:\n%s", get_time(), (char *) data);
            break;
        case I_RACE_START:
            snprintf(entry, LARGEST_SIZE, "%s => RACE STARTED", get_time());
            break;
        case I_CANNOT_START:
            snprintf(entry, LARGEST_SIZE, "%s => RACE CANNOT START", get_time());
            break;
        case I_CAR_MALFUNCTION:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s FROM TEAM %s SUFFERED MALFUNCTION", get_time(), car->name,
                     car->team->team_name);
            break;
        case I_SIGNAL_RECEIVED:
            snprintf(entry, LARGEST_SIZE, "%s => SIGNAL %s RECEIVED", get_time(), (char *) data);
            break;
        case I_RACE_WIN:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s FROM TEAM %s WON", get_time(), car->name,
                     car->team->team_name);
            break;
        case I_COMMAND_REJECTED_2:
            snprintf(entry, LARGEST_SIZE, "%s => RACE ALREADY BEGAN! COMMAND REJECTED:\n'%s'", get_time(), (char *) data);
            break;
        case I_BOX_REFUEL:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s ENTERED TEAM BOX FOR REFUELLING", get_time(), car->name);
            break;
        case I_BOX_MALFUNCTION:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s ENTERED TEAM BOX DUE TO MALFUNCTION", get_time(), car->name);
            break;
        case I_BOX_LEFT:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s LEFT TEAM BOX", get_time(), car->name);
            break;
        case I_CAR_RAN_OUT_OF_FUEL:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s FROM TEAM %s RAN OUT OF FUEL THEREFORE IS DISQUALIFIED", get_time(), car->name, car->team->team_name);
            break;
        default:
            throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);
            break;
    }

    printf("%s\n", entry);
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