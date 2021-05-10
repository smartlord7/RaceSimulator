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

#define I_SIMULATION_START "SIMULATION START"
#define I_SIMULATION_END "SIMULATION END"
#define I_COMMAND_RECEIVED "COMMAND RECEIVED"
#define I_COMMAND_EXCEPTION "WRONG COMMAND"
#define I_CAR_LOADED "CAR LOADED"
#define I_CAR_REJECTED "CAR REJECTED"
#define I_CANNOT_START "CANNOT START RACE"
#define I_CAR_MALFUNCTION "CAR MALFUNCTION"
#define I_SIGNAL_RECEIVED "SIGNAL RECEIVED"
#define I_RACE_WIN "RACE WIN"

// endregion log_gen input modes

// region log_gen output msgs

#define O_SIMULATION_STARTING " SIMULATOR STARTING"
#define O_SIMULATION_CLOSING " SIMULATOR CLOSING"
#define O_NEW_COMMAND_RECEIVED " NEW COMMAND RECEIVED: "
#define O_WRONG_COMMAND " WRONG COMMAND => "
#define O_CAR_LOADED " CAR LOADED => "
#define O_CAR_REJECTED " CAR REJECTED =>"
#define O_CANNOT_START " CANNOT START RACE! STILL MISSING CONDITIONS!"
#define O_TEMP_NUM " X "
#define O_NEW_CAR_PROBLEM " NEW PROBLEM IN CAR"
#define O_SIGNAL " SIGNAL "
#define O_RECEIVED " RECEIVED"
#define O_CAR " CAR "
#define O_RACE_WON " WINS THE RACE"

// endregion log_gen output msgs

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
void generate_log_entry(char * mode, void * data);

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