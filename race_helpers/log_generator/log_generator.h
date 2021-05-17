/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACE_SIMULATOR_C_LOG_GENERATOR_H
#define RACE_SIMULATOR_C_LOG_GENERATOR_H

// region constants

// region log_gen input modes

typedef enum log_msg {
    ERROR_MISSING_CAR_ATTR,
    ERROR_INVALID_CAR_ATTR,
    ERROR_MISSING_CAR_ATTR_VALUE,
    ERROR_INVALID_CAR_ATTR_VALUE,
    ERROR_NOT_ENOUGH_TEAMS,
    ERROR_TOO_MANY_TEAMS,
    ERROR_TOO_MANY_CARS,
    ERROR_UNIQUE_CONSTRAINT_VIOLATED,
    SIMULATION_START,
    SIMULATION_END,
    RACE_START,
    RACE_FINISH,
    RACE_CANNOT_START,
    COMMAND_RECEIVE,
    COMMAND_REJECT,
    COMMAND_REJECT2,
    CAR_REJECT,
    SIGNAL_RECEIVE,
    CAR_LOAD,
    CAR_MALFUNCTION,
    CAR_STATE_CHANGE,
    CAR_RACE_WIN,
    CAR_OUT_OF_FUEL,
    CAR_COMPLETE_LAP,
    CAR_FINISH,
    BOX_STATE_CHANGE,
    BOX_FIX,
    BOX_REFUEL,
    BOX_REFUEL_ENTER,
    BOX_MALFUNCTION_ENTER,
    BOX_LEAVE
} log_msg;

// endregion log_gen input modes

#define FACTOR 4
#define HEADER " ----- Race Simulator -----\nDevelopers:\n - Joao Filipe Guiomar Artur, 2019217853\n - Sancho Amaral Simoes, 2019217590\nOperating Systems, LEI, FCTUC, 2020/2021\n\n"
#define TIMESTAMP_DELIMITER " "
#define OF_TIMESTAMP " OF TIMESTAMP"
#define NUM_TIMESTAMP_FIELDS 3
#define true 1
#define false 0

// endregion constants

// region public functions prototypes

/**
 * @def generate_log_entry
 * @brief Function that generates a log entry. This entry is then printed to the screen and written to the specified log file synchronously.
 *
 * @param mode
 * The mode in which the function is called.
 *
 * @param main_data
 * Additional data needed for some cases.
 *
 */
void generate_log_entry(log_msg mode, void *main_data, void *sec_data);

/**
 * @def log_init
 * @brief Function that initializes the log generator and opens log file.
 *
 * @param lg_file_path
 * The path of the log file.
 *
 * @param sem
 * A pointer to a POSIX named semaphore that allows a synchronized output.
 *
 */
void log_init(const char * lg_file_path);

/**
 * @def log_init
 * @brief Function that closes the log file.
 */
void log_close();

// endregion public functions prototypes

// region global variables

extern const char * log_file_path;

// endregion global variables

#endif //RACE_SIMULATOR_C_LOG_GENERATOR_H