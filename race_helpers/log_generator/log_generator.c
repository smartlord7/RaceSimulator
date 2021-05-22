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
char * mmap;
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

void generate_log_entry(log_msg_type mode, void * main_data, void * sec_data) {
    char entry[LARGE_SIZE], * aux = NULL;
    snprintf(entry, LARGE_SIZE, "%s => ", get_curr_time_as_str());

    if (mode < EXIT_PROGRAM) {
        switch (mode) {
            case ERROR_MISSING_CAR_ATTR:
                append_f(entry, "ERROR: MISSING CAR ATTRIBUTE NAMED '%s'!", (char *) main_data);
                break;
            case ERROR_INVALID_CAR_ATTR:
                append_f("ERROR: CAR HAS NO ATTRIBUTE NAMED '%s'!", (char *) main_data);
                break;
            case ERROR_MISSING_CAR_ATTR_VALUE:
                append_f(entry, "ERROR: MISSING VALUE OF CAR ATTRIBUTE '%s'!", (char *) main_data);
                break;
            case ERROR_INVALID_CAR_ATTR_VALUE:
                append_f(entry, "ERROR: INVALID VALUE '%s' FOR CAR ATTRIBUTE '%s'!", (char *) main_data, (char *) sec_data);
                break;
            case ERROR_NOT_ENOUGH_TEAMS:
                append_f(entry, "ERROR: NOT ENOUGH TEAMS! THE NEEDED NUMBER IS %d!", config.num_teams);
                break;
            case ERROR_TOO_MANY_TEAMS:
                append_f(entry, "ERROR: TOO MANY TEAMS! THE MAXIMUM NUMBER IS %d!", config.num_teams);
                break;
            case ERROR_TOO_MANY_CARS:
                append_f(entry, "ERROR: TOO MANY CARS IN TEAM! THE MAXIMUM NUMBER IS %d!", config.max_cars_per_team);
                break;
            case ERROR_UNIQUE_CONSTRAINT_VIOLATED:
                append_f(entry, "ERROR: CAR PARAM '%s' WITH VALUE '%s' ALREADY EXISTS!", (char *) main_data, (char *) sec_data);
                break;
            default:
                throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);
                break;
        }
    } else if (mode < CAR_LOAD) {
        switch (mode) {
            case EXIT_PROGRAM:
                append(entry, "SIMULATION CLOSED BY USER! EXITING...");
                break;
            case SIMULATION_START:
                append(entry, "STARTING SIMULATION...");
                break;
            case SIMULATION_END:
                append(entry, "ENDING SIMULATION...");
                break;
            case CLOCK:
                append_f(entry, "-------------------[GLOBAL CLOCK] %d tu-------------------", shm->sync_s.global_time);
                break;
            case RACE_START:
                append_f(entry, "THE RACE HAS STARTED!");
                break;
            case RACE_FINISH:
                append_f(entry, "THE RACE HAS FINISHED AT %d tu!", shm->sync_s.global_time);
                break;
            case RACE_NOT_STARTED:
                append_f(entry, "%s! RACE HAS NOT STARTED!", (char *) main_data);
                break;
            case RACE_CANNOT_START:
                append_f(entry, "THE RACE CANNOT START!");
                break;
            case COMMAND_RECEIVE:
                append_f(entry, "COMMAND RECEIVED: \n '%s'", (char *) main_data);
                break;
            case COMMAND_REJECT:
                append_f(entry, "COMMAND REJECTED: \n'%s'", (char *) main_data);
                break;
            case COMMAND_REJECT2:
                append_f(entry, "THE RACE HAS ALREADY BEGAN! COMMAND REJECTED:\n'%s'", (char *) main_data);
                break;
            case SIGNAL_RECEIVE:
                append_f(entry, "SIGNAL %s RECEIVED!", (char *) main_data);
                break;
            case CAR_REJECT:
                append_f(entry, "CAR REJECTED: \n%s", (char *) main_data);
                break;
            default:
                throw_and_stay(LOG_MODE_NOT_SUPPORTED_EXCEPTION, mode);
                break;
        }
    } else if (mode < BOX_STATE_CHANGE){
        race_car_t * car = (race_car_t *) main_data;

        switch (mode) {
            case CAR_LOAD:
                append_f(entry, "CAR %s FROM TEAM '%s' LOADED", car->name, car->team->team_name);
                break;
            case CAR_MALFUNCTION:
                append_f(entry, "CAR %s FROM TEAM %s IS MALFUNCTIONING!\n"
                                "MALFUNCTION DESCRIPTION: %s", car->name, car->team->team_name, (char *) sec_data);
                break;
            case CAR_STATE_CHANGE:
                aux = race_car_state_to_string(car->state);
                append_f(entry, "CAR %s CHANGED TO STATE TO '%s'!", car->name, aux);
                break;
            case CAR_RACE_WIN:
                append_f(entry, "CAR %s FROM TEAM %s WON THE RACE AT %d tu!", car->name, car->team->team_name,
                         shm->sync_s.global_time);
                break;
            case CAR_OUT_OF_FUEL:
                append_f(entry, "CAR %s FROM TEAM %s RAN OUT OF FUEL!", car->name, car->team->team_name);
                break;
            case CAR_COMPLETE_LAP:
                append_f(entry, "CAR %s HAS COMPLETED %d LAPS!", car->name, car->completed_laps);
                break;
            case CAR_FINISH:
                append_f(entry, "CAR %s FROM TEAM %s HAS FINISHED THE RACE AT %d tu!", car->name, car->team->team_name,
                         shm->sync_s.global_time);
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
                append_f(entry, "BOX FROM TEAM %s HAS CHANGED TO STATE '%s'!", box->team->team_name, aux);
                break;
            case BOX_REFUEL_ENTER:
                append_f(entry, "CAR %s ENTERED TEAM BOX FOR REFUELLING!", box->current_car->name);
                break;
            case BOX_MALFUNCTION_ENTER:
                append_f(entry, "CAR %s ENTERED TEAM BOX DUE TO MALFUNCTION!", box->current_car->name);
                break;
            case BOX_FIX:
                append_f(entry, "FIXING CAR %s...", box->current_car->name);
                break;
            case BOX_REFUEL:
                append_f(entry, "REFUELING CAR %s...", box->current_car->name);
                break;
            case BOX_LEAVE:
                append_f(entry, "CAR %s LEFT TEAM BOX AND IS NOW READY TO RACE!", box->current_car->name);
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