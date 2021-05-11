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

// region dependencies

#include <semaphore.h>

// endregion dependencies

// region constants

// region log_gen input modes

#define I_SIMULATION_START 0
#define I_SIMULATION_END 1
#define I_COMMAND_RECEIVED 2
#define I_COMMAND_EXCEPTION 3
#define I_CAR_LOADED 4
#define I_CAR_REJECTED 5
#define I_RACE_START 6
#define I_CANNOT_START 7
#define I_CAR_MALFUNCTION 8
#define I_SIGNAL_RECEIVED 9
#define I_RACE_WIN 10

// endregion log_gen input modes

#define TIMESTAMP_DELIMITER " "
#define OF_TIMESTAMP " OF TIMESTAMP"
#define NUM_TIMESTAMP_FIELDS 3
#define MUTEX "MUTEX"
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
 * @param data
 * Additional data needed for some cases.
 *
 */
void generate_log_entry(int mode, void * data);

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