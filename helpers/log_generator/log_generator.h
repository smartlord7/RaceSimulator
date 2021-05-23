/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACE_HELPERS_LOG_GENERATOR_H
#define RACE_HELPERS_LOG_GENERATOR_H

// region constants

#define MMAP_FILE_INITIAL_SEEK_FACTOR 4
#define LOG_FILE_HEADER " ----- Race Simulator -----\nDevelopers:\n - Joao Filipe Guiomar Artur, 2019217853\n - Sancho Amaral Simoes, 2019217590\nOperating Systems, LEI, FCTUC, 2020/2021\n\n"
#define TIMESTAMP_DELIMITER " "
#define NUM_TIMESTAMP_FIELDS 3
#define true 1
#define false 0

#define ERROR "ERROR: "
#define RACE_SIMULATOR_SAYS "[RACE SIMULATOR] "
#define RACE_MANAGER_SAYS "[RACE MANAGER] "
#define GLOBAL_CLOCK_SAYS "[GLOBAL CLOCK] "
#define RACE_CAR_SAYS "[RACE CAR %s] "
#define TEAM_MANAGER_SAYS "[TEAM %s] "

// region error log msgs

#define MISSING_CAR_ATTR_ ERROR "MISSING CAR ATTRIBUTE NAMED '%s'!"
#define NO_CAR_ATTR_ "ERROR CAR HAS NO ATTRIBUTE AT THIS POSITION NAMED '%s'!"
#define MISSING_CAR_ATTR_VALUE_ ERROR "MISSING VALUE OF CAR ATTRIBUTE '%s'!"
#define INVALID_CAR_ATTR_VALUE_ ERROR "INVALID VALUE '%s' FOR CAR ATTRIBUTE '%s'!"
#define NOT_ENOUGH_TEAMS_ ERROR "NOT ENOUGH TEAMS! THE EXPECTED NUMBER IS %d!"
#define TOO_MANY_TEAMS_ ERROR "TOO MANY TEAMS! THE MAXIMUM NUMBER IS %d!"
#define TOO_MANY_CARS_ ERROR "TOO MANY CARS IN TEAM! THE MAXIMUM NUMBER IS %d!"
#define UNIQUE_CONSTRAINT_VIOLATED_ ERROR "CAR PARAM '%s' WITH VALUE '%s' ALREADY EXISTS!"

// endregion error log msgs

// region race general msgs

#define SIMULATION_CLOSE_ RACE_MANAGER_SAYS "SIMULATION CLOSED BY USER! EXITING..."
#define SIMULATION_START_ RACE_SIMULATOR_SAYS "STARTING SIMULATION..."
#define SIMULATION_END_ RACE_SIMULATOR_SAYS "ENDING SIMULATION..."
#define CLOCK_ GLOBAL_CLOCK_SAYS "%d tu-------------------------------"
#define RACE_START_ RACE_MANAGER_SAYS "THE RACE HAS STARTED!"
#define RACE_FINISH_ RACE_MANAGER_SAYS "THE RACE HAS FINISHED AT %d tu!"
#define RACE_NOT_STARTED_ RACE_MANAGER_SAYS "THE RACE HAS NOT STARTED!"
#define RACE_CANNOT_START_ RACE_MANAGER_SAYS "ERROR: THE RACE CANNOT START!"
#define COMMAND_RECEIVE_ RACE_MANAGER_SAYS "COMMAND RECEIVED:\n'%s'"
#define COMMAND_REJECT_ RACE_MANAGER_SAYS "COMMAND REJECTED:\n'%s'"
#define COMMAND_REJECT2_ RACE_MANAGER_SAYS "THE RACE HAS ALREADY BEGAN! COMMAND REJECTED:\n'%s'"
#define SIGNAL_RECEIVE_ RACE_MANAGER_SAYS "SIGNAL %s RECEIVED!"
#define CAR_REJECT_ RACE_MANAGER_SAYS "CAR REJECTED:\n'%s'"

// endregion race general msgs

// region car msgs

#define CAR_LOADED_ RACE_CAR_SAYS "LOADED TO TEAM SLOT %d!"
#define CAR_MALFUNCTION_ RACE_CAR_SAYS "A MALFUNCTION WAS DETECTED! %s"
#define CAR_STATE_CHANGE_ RACE_CAR_SAYS "CHANGED TO STATE '%s'!"
#define CAR_MIN_FUEL1_REACHED_ RACE_CAR_SAYS "NOT ENOUGH FUEL TO COMPLETE %d MORE LAPS! BOX ACCESS IS NEEDED!"
#define CAR_MIN_FUEL2_REACHED_ RACE_CAR_SAYS "THE FUEL LEVEL IS DANGEROUSLY LOW! BOX ACCESS IS CRUCIAL!"
#define CAR_RACE_WIN_ RACE_CAR_SAYS "WON THE RACE AT %d tu!"
#define CAR_OUT_OF_FUEL_ RACE_CAR_SAYS "RAN OUT OF FUEL!"
#define CAR_COMPLETE_LAP_ RACE_CAR_SAYS "COMPLETED %d LAPS!"
#define CAR_FINISH_ RACE_CAR_SAYS "FINISHED THE RACE AT %d tu!"

// endregion car msgs

// region box msgs

#define BOX_STATE_CHANGE_ TEAM_MANAGER_SAYS "BOX CHANGED TO STATE '%s'!"
#define BOX_REFUEL_ENTER_ TEAM_MANAGER_SAYS "CAR %s ENTERED BOX FOR REFUELING!"
#define BOX_MALFUNCTION_ENTER_ TEAM_MANAGER_SAYS "CAR %s ENTERED BOX DUE TO MALFUNCTION!"
#define BOX_REPAIR_ TEAM_MANAGER_SAYS "REPAIRING CAR %s..."
#define BOX_REFUEL_ TEAM_MANAGER_SAYS "REFUELING CAR %s..."
#define BOX_LEAVE_ "CAR %s LEFT THE BOX AND IS NOW READY TO RACE!"

// endregion box msgs

// endregion constants

// region log_gen input modes

/**
 * @enum log_msg_type
 * @brief Enum that lists all the possible logging options.
 * This method is used since it offers natural order and allows to use integers instead of strings, which are much heavier.
 *
 */
typedef enum log_msg_type {
    ERROR_MISSING_CAR_ATTR,
    ERROR_INVALID_CAR_ATTR,
    ERROR_MISSING_CAR_ATTR_VALUE,
    ERROR_INVALID_CAR_ATTR_VALUE,
    ERROR_NOT_ENOUGH_TEAMS,
    ERROR_TOO_MANY_TEAMS,
    ERROR_TOO_MANY_CARS,
    ERROR_UNIQUE_CONSTRAINT_VIOLATED,
    EXIT_PROGRAM,
    SIMULATION_START,
    SIMULATION_END,
    CLOCK,
    RACE_START,
    RACE_FINISH,
    RACE_NOT_STARTED,
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
    CAR_MIN_FUEL1_REACHED,
    CAR_MIN_FUEL2_REACHED,
    CAR_OUT_OF_FUEL,
    CAR_COMPLETE_LAP,
    CAR_FINISH,
    BOX_STATE_CHANGE,
    BOX_REPAIR,
    BOX_REFUEL,
    BOX_REFUEL_ENTER,
    BOX_MALFUNCTION_ENTER,
    BOX_LEAVE
} log_msg_type;

// endregion log_gen input modes

// region public functions prototypes

/**
 * @def generate_log_entry
 * @brief Function that generates a log entry. This entry is then printed to the screen and written to the specified log file synchronously.
 *
 * @param type
 * The log msg type in which the function is called.
 *
 * @param main_data
 * Primary data needed for some cases.
 *
 * @param sec_data
 * Secondary data needed for some cases.
 *
 */
extern void generate_log_entry(log_msg_type type, void * main_data, void * sec_data);

/**
 * @def log_init
 * @brief Function that initializes the log generator and opens the log file.
 *
 * @param lg_file_path
 * The path of the log file.
 *
 */
extern void log_init(const char * lg_file_path);

/**
 * @def log_init
 * @brief Function that closes the log file.
 */
extern void log_close();

// endregion public functions prototypes

// region global variables

extern const char * log_file_path;

// endregion global variables

#endif //RACE_HELPERS_LOG_GENERATOR_H