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
#include "../../util/strings/strings.h"

// endregion dependencies

#define FACTOR 4
#define HEADER " ----- Race Simulator -----\nDevelopers:\n - Joao Filipe Guiomar Artur, 2019217853\n - Sancho Amaral Simoes, 2019217590\nOperating Systems, LEI, FCTUC, 2020/2021\n\n"

// region private functions prototypes

/**
 * @def get_curr_time_as_str
 * @brief Function that gives the current time in the hh:mm:ss format.
 *
 * @return Current local system time in the hh:mm:ss format.
 */
char * get_curr_time_as_str();

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
    char entry[LARGE_SIZE];
    snprintf(entry, LARGE_SIZE, "%s => ", get_curr_time_as_str());

    switch (mode) {
        case I_SIMULATION_START:
            append(entry, "STARTING SIMULATION");
            break;
        case I_SIMULATION_END:
            append(entry, "ENDING SIMULATION");
            break;
        case I_COMMAND_RECEIVED:
            append_f(entry, "COMMAND RECEIVED: \n '%s'", (char *) data);
            break;
        case I_COMMAND_REJECTED:
            append_f(entry, "COMMAND REJECTED: \n'%s'", (char *) data);
            break;
        case I_CAR_LOADED:
            car = (race_car_t *) data;
            append_f(entry, "CAR %s FROM TEAM '%s' LOADED", car->name, car->team->team_name);
            break;
        case I_CAR_REJECTED:
            append_f(entry, "CAR REJECTED: \n%s", (char *) data);
            break;
        case I_RACE_START:
            append_f(entry, "THE RACE HAS STARTED!");
            break;
        case I_CANNOT_START:
            append_f(entry, "THE RACE CANNOT START!");
            break;
        case I_CAR_MALFUNCTION:
            car = (race_car_t *) data;
            append_f(entry, "CAR %s FROM TEAM %s IS MALFUNCTIONING!", car->name, car->team->team_name);
            break;
        case I_SIGNAL_RECEIVED:
            append_f(entry, "SIGNAL %s RECEIVED!", (char *) data);
            break;
        case I_RACE_WIN:
            car = (race_car_t *) data;
            append_f(entry, "CAR %s FROM TEAM %s WON THE RACE AT %d tu!", car->name, car->team->team_name, shm->sync_s.global_time);
            break;
        case I_COMMAND_REJECTED_2:
            append_f(entry, "THE RACE HAS ALREADY BEGAN! COMMAND REJECTED:\n'%s'", (char *) data);
            break;
        case I_BOX_REFUEL:
            car = (race_car_t *) data;
            append_f(entry, "CAR %s ENTERED TEAM BOX FOR REFUELLING!", car->name);
            break;
        case I_BOX_MALFUNCTION:
            car = (race_car_t *) data;
            append_f(entry, "CAR %s ENTERED TEAM BOX DUE TO MALFUNCTION!", car->name);
            break;
        case I_BOX_LEFT:
            car = (race_car_t *) data;
            append_f(entry, "CAR %s LEFT TEAM BOX AND IS NOW READY TO RACE!", car->name);
            break;
        case I_CAR_RAN_OUT_OF_FUEL:
            car = (race_car_t *) data;
            append_f(entry, "CAR %s FROM TEAM %s RAN OUT OF FUEL!", car->name, car->team->team_name);
            break;
        case I_CAR_FINISH:
            car = (race_car_t *) data;
            append_f(entry, "CAR %s FROM TEAM %s HAS FINISHED THE RACE AT %d tu!", car->name, car->team->team_name, shm->sync_s.global_time);
            break;
        default:
            throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);
            break;
    }

    append(entry, "\n");
    printf("%s", entry);
    write_stream(log_fd, entry, strlen(entry));
}

// endregion public functions

// region private functions

char * get_curr_time_as_str(){
    time_t current_time;
    char * buffer, * token, * result;
    int i;

    current_time = time(NULL);
    buffer = asctime(localtime(&current_time));

    if ((token = strtok_r(buffer, TIMESTAMP_DELIMITER, &buffer)) == NULL) {
        throw_and_exit(TOKENIZE_EXCEPTION, OF_TIMESTAMP);
    }

    for(i = 0; i < NUM_TIMESTAMP_FIELDS; i++){
        if ((token = strtok_r(NULL, TIMESTAMP_DELIMITER, &buffer)) == NULL) {
            throw_and_exit(TOKENIZE_EXCEPTION, OF_TIMESTAMP);
        }
    }

    result = token;

    return result;
}

// endregion private functions