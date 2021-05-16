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
#include <fcntl.h>
#include <unistd.h>
#include "../../util/exception_handler/exception_handler.h"
#include "log_generator.h"
#include "../../util/exception_handler/exception_handler.h"
#include "../../util/global.h"
#include "../../ipcs/memory_mapped_file/memory_mapped_file.h"
#include "../../util/file/file.h"

// endregion dependencies

#define FACTOR 4
#define HEADER " ----- Race Simulator -----\nDevelopers:\n - Joao Filipe Guiomar Artur, 2019217853\n - Sancho Amaral Simoes, 2019217590\nOperating Systems, LEI, FCTUC, 2020/2021\n"

// region private functions prototypes

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
char * mmap;
size_t file_size;
int log_fd;

// endregion global variables

// region public functions

void log_init(const char * lg_file_path) {

    log_fd = open(lg_file_path, O_RDWR | O_CREAT, 0600);
    throw_if_exit(log_fd == -1, FILE_OPEN_EXCEPTION, lg_file_path);

    lseek(log_fd, FACTOR * LARGEST_SIZE - 1, SEEK_SET);
    lseek(log_fd, 0, SEEK_SET);

    write_stream(log_fd, HEADER, sizeof(HEADER) - sizeof(char));

    //mmap = create_mmap_file(log_fd, &file_size);

    log_file_path = lg_file_path;
}

void log_close(){
    //destroy_mmap(mmap, log_fd, file_size);

    if (close(log_fd) < 0){
        throw_and_exit(FILE_CLOSE_EXCEPTION, log_file_path);
    }

}

void generate_log_entry(int mode, void * data){
    race_car_t * car;
    char entry[LARGEST_SIZE];

    switch (mode) {
        case I_SIMULATION_START:
            snprintf(entry, LARGEST_SIZE, "%s => STARTING SIMULATION\n", get_time());
            break;
        case I_SIMULATION_END:
            snprintf(entry, LARGEST_SIZE, "%s => CLOSING SIMULATION\n", get_time());
            break;
        case I_COMMAND_RECEIVED:
            snprintf(entry, LARGEST_SIZE, "%s => COMMAND RECEIVED:\n'%s'\n", get_time(), (char *) data);
            break;
        case I_COMMAND_REJECTED:
            snprintf(entry, LARGEST_SIZE, "%s => COMMAND REJECTED:\n'%s'\n", get_time(), (char *) data);
            break;
        case I_CAR_LOADED:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s FROM TEAM '%s' LOADED\n", get_time(), car->name,
                     car->team->team_name);
            break;
        case I_CAR_REJECTED:
            snprintf(entry, LARGEST_SIZE, "%s => CAR REJECTED:\n%s\n", get_time(), (char *) data);
            break;
        case I_RACE_START:
            snprintf(entry, LARGEST_SIZE, "%s => RACE STARTED\n", get_time());
            break;
        case I_CANNOT_START:
            snprintf(entry, LARGEST_SIZE, "%s => RACE CANNOT START\n", get_time());
            break;
        case I_CAR_MALFUNCTION:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s FROM TEAM %s SUFFERED MALFUNCTION\n", get_time(), car->name,
                     car->team->team_name);
            break;
        case I_SIGNAL_RECEIVED:
            snprintf(entry, LARGEST_SIZE, "%s => SIGNAL %s RECEIVED\n", get_time(), (char *) data);
            break;
        case I_RACE_WIN:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s FROM TEAM %s WON\n", get_time(), car->name,
                     car->team->team_name);
            break;
        case I_COMMAND_REJECTED_2:
            snprintf(entry, LARGEST_SIZE, "%s => RACE ALREADY BEGAN! COMMAND REJECTED:\n'%s'\n", get_time(), (char *) data);
            break;
        case I_BOX_REFUEL:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s ENTERED TEAM BOX FOR REFUELLING\n", get_time(), car->name);
            break;
        case I_BOX_MALFUNCTION:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s ENTERED TEAM BOX DUE TO MALFUNCTION\n", get_time(), car->name);
            break;
        case I_BOX_LEFT:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s LEFT TEAM BOX\n", get_time(), car->name);
            break;
        case I_CAR_RAN_OUT_OF_FUEL:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s FROM TEAM %s RAN OUT OF FUEL THEREFORE IS DISQUALIFIED\n", get_time(), car->name, car->team->team_name);
            break;
        case I_CAR_FINISH:
            car = (race_car_t *) data;
            snprintf(entry, LARGEST_SIZE, "%s => CAR %s FROM TEAM %s HAS FINISHED THE RACE\n", get_time(), car->name, car->team->team_name);
            break;
        default:
            throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);
            break;
    }

    printf("%s", entry);
    write_stream(log_fd, entry, strlen(entry));
}

// endregion public functions

// region private functions

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