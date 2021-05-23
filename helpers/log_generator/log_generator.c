/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/


// region dependencies

#include "stdio.h"
#include "string.h"
#include "time.h"
#include "fcntl.h"
#include "unistd.h"
#include "malloc.h"
#include "../../util/exception_handler/exception_handler.h"
#include "../../util/global.h"
#include "../../util/file/file.h"
#include "../../util/strings/strings.h"
#include "log_generator.h"
#include "../../ipcs/memory_mapped_file/memory_mapped_file.h"
#include "../../processes/team_manager/team_manager.h"

// endregion dependencies

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
char * mmap_f;
static int log_fd;
static size_t log_file_size;

// endregion global variables

// region public functions

void log_init(const char * lg_file_path) {

    log_fd = open(lg_file_path, O_RDWR | O_CREAT, 0600);
    throw_if_exit(log_fd == -1, FILE_OPEN_EXCEPTION, lg_file_path);

    lseek(log_fd, MMAP_FILE_INITIAL_SEEK_FACTOR * LARGEST_SIZE - 1, SEEK_SET);
    lseek(log_fd, 0, SEEK_SET);

    write_stream(log_fd, HEADER, sizeof(HEADER) - sizeof(char));

    //mmap_f = create_mmap_file(log_fd, &log_file_size);

    log_file_path = lg_file_path;
}

void log_close(){
    //destroy_mmap_file(mmap_f, log_fd, log_file_size);

    if (close(log_fd) < 0){
        throw_and_exit(FILE_CLOSE_EXCEPTION, log_file_path);
    }

}

void generate_log_entry(log_msg_type mode, void * main_data, void * sec_data) {
    char entry[4 * LARGEST_SIZE], * aux = NULL;
    snprintf(entry, LARGE_SIZE, "%s => ", get_curr_time_as_str());

    if (mode < EXIT_PROGRAM) {
        switch (mode) {
            case ERROR_MISSING_CAR_ATTR:
                append_f(entry, MISSING_CAR_ATTR_, (char *) main_data);
                break;
            case ERROR_INVALID_CAR_ATTR:
                append_f(NO_CAR_ATTR_, (char *) main_data);
                break;
            case ERROR_MISSING_CAR_ATTR_VALUE:
                append_f(entry, MISSING_CAR_ATTR_VALUE_, (char *) main_data);
                break;
            case ERROR_INVALID_CAR_ATTR_VALUE:
                append_f(entry, INVALID_CAR_ATTR_VALUE_, (char *) main_data, (char *) sec_data);
                break;
            case ERROR_NOT_ENOUGH_TEAMS:
                append_f(entry, NOT_ENOUGH_TEAMS_, config.num_teams);
                break;
            case ERROR_TOO_MANY_TEAMS:
                append_f(entry, TOO_MANY_TEAMS_, config.num_teams);
                break;
            case ERROR_TOO_MANY_CARS:
                append_f(entry, TOO_MANY_CARS_, config.max_cars_per_team);
                break;
            case ERROR_UNIQUE_CONSTRAINT_VIOLATED:
                append_f(entry, UNIQUE_CONSTRAINT_VIOLATED_, (char *) main_data, (char *) sec_data);
                break;
            default:
                throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);
                break;
        }
    } else if (mode < CAR_LOAD) {
        switch (mode) {
            case EXIT_PROGRAM:
                append(entry, SIMULATION_CLOSE_);
                break;
            case SIMULATION_START:
                append(entry, SIMULATION_START_);
                break;
            case SIMULATION_END:
                append(entry, SIMULATION_END_);
                break;
            case CLOCK:
                append_f(entry, CLOCK_, shm->thread_clock.global_time);
                break;
            case RACE_START:
                append_f(entry, RACE_START_);
                break;
            case RACE_FINISH:
                append_f(entry, RACE_FINISH_, shm->thread_clock.global_time);
                break;
            case RACE_NOT_STARTED:
                append_f(entry, RACE_NOT_STARTED_, (char *) main_data);
                break;
            case RACE_CANNOT_START:
                append_f(entry, RACE_CANNOT_START_);
                break;
            case COMMAND_RECEIVE:
                append_f(entry, COMMAND_RECEIVE_, (char *) main_data);
                break;
            case COMMAND_REJECT:
                append_f(entry, COMMAND_REJECT_, (char *) main_data);
                break;
            case COMMAND_REJECT2:
                append_f(entry, COMMAND_REJECT2_, (char *) main_data);
                break;
            case SIGNAL_RECEIVE:
                append_f(entry, SIGNAL_RECEIVE_, (char *) main_data);
                break;
            case CAR_REJECT:
                append_f(entry, CAR_REJECT_, (char *) main_data);
                break;
            default:
                throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);
                break;
        }
    } else if (mode < BOX_STATE_CHANGE){
        race_car_t * car = (race_car_t *) main_data;

        switch (mode) {
            case CAR_LOAD:
                append_f(entry, CAR_LOADED_, car->name, car->team->team_id);
                break;
            case CAR_MALFUNCTION:
                append_f(entry, CAR_MALFUNCTION_, car->name, (char *) sec_data);
                break;
            case CAR_STATE_CHANGE:
                aux = race_car_state_to_string(car->state);
                append_f(entry, CAR_STATE_CHANGE_, car->name, aux);
                break;
            case CAR_RACE_WIN:
                append_f(entry, CAR_RACE_WIN_, car->name, shm->thread_clock.global_time);
                break;
            case CAR_MIN_FUEL1_REACHED:
                append_f(entry, CAR_MIN_FUEL1_REACHED_, car->name, REFUEL_MIN_LAPS1);
                break;
            case CAR_MIN_FUEL2_REACHED:
                append_f(entry, CAR_MIN_FUEL2_REACHED_, car->name);
                break;
            case CAR_OUT_OF_FUEL:
                append_f(entry, CAR_OUT_OF_FUEL_, car->name);
                break;
            case CAR_COMPLETE_LAP:
                append_f(entry, CAR_COMPLETE_LAP_, car->name, car->completed_laps);
                break;
            case CAR_FINISH:
                append_f(entry, CAR_FINISH_, car->name, shm->thread_clock.global_time);
                break;
            default:
                throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);
                break;
        }
    } else {
        race_box_t * box = (race_box_t *) main_data;

        switch (mode) {
            case BOX_STATE_CHANGE:
                aux = race_box_state_to_string(box->state);
                append_f(entry, BOX_STATE_CHANGE_, box->team->team_name, aux);
                break;
            case BOX_REFUEL_ENTER:
                append_f(entry, BOX_REFUEL_ENTER_, box->team->team_name, box->current_car->name);
                break;
            case BOX_MALFUNCTION_ENTER:
                append_f(entry, BOX_MALFUNCTION_ENTER_, box->team->team_name, box->current_car->name);
                break;
            case BOX_REPAIR:
                append_f(entry, BOX_REPAIR_, box->team->team_name, box->current_car->name);
                break;
            case BOX_REFUEL:
                append_f(entry, BOX_REFUEL_, box->team->team_name, box->current_car->name);
                break;
            case BOX_LEAVE:
                append_f(entry, BOX_LEAVE_, box->team->team_name, box->current_car->name);
                break;
            default:
                throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);
                break;
        }
     }

    if (aux != NULL) {
        free(aux);
    }

    append(entry, "\n");
    lock_mutex(&shm->stdout_mutex);
    printf("%s", entry);
    unlock_mutex(&shm->stdout_mutex);

    lock_mutex(&shm->log_mutex);
    write_stream(log_fd, entry, strlen(entry));
    unlock_mutex(&shm->log_mutex);
}

// endregion public functions

// region private functions

char * get_curr_time_as_str(){
    time_t current_time;
    char * buffer = NULL, * token = NULL, * result = NULL;
    int i;

    current_time = time(NULL);
    buffer = asctime(localtime(&current_time));

    token = strtok_r(buffer, TIMESTAMP_DELIMITER, &buffer);

    for(i = 0; i < NUM_TIMESTAMP_FIELDS; i++){
        token = strtok_r(NULL, TIMESTAMP_DELIMITER, &buffer);
    }

    result = token;

    return result;
}

// endregion private functions