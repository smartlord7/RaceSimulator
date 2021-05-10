/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACESIMULATOR_C_DEBUG_H
#define RACESIMULATOR_C_DEBUG_H

// region dependencies

#include <semaphore.h>

// endregion dependencies

// region constants


#define DEBUG 1

typedef enum debug_level {
    SETUP = 0,
    ENTRY = 1,
    PARAM = 2,
    EVENT = 3,
    COMMS = 4,
    GENERAL = 5
} debug_level;


// region ipcs debug msgs

#define SHM_CREATE "CREATED SHARED MEMORY WITH ID %d!"
#define SHM_ATTACH "ATTACHED SHARED MEMORY WITH ID %d TO CURRENT PROCESS ADDRESSING ZONE!"
#define SHM_DETACH "DETACHED SHARED MEMORY WITH ID %d!"
#define SHM_REMOVED "REMOVED SHARED MEMORY WITH ID %d!"
#define SEM_CREATE "CREATED SEMAPHORE NAMED %s!"
#define SEM_CLOSE "CLOSED SEMAPHORE NAMED %s!"
#define SEM_UNLINK "UNLINKED SEMAPHORE NAMED %s!"
#define IPCS_CREATE "CREATED IPCS!"
#define COND_VAR_CREATE "CREATED CONDITION VARIABLE %s!"
#define COND_VAR_DESTROY "DESTROYED CONDITION VARIABLE %s"
#define MUTEX_CREATE "CREATED MUTEX %s!"
#define MUTEX_DESTROY "DESTROYED MUTEX %s!"
#define PIPE_NEW_MSG "NEW MESSAGE FROM PIPE %d!"

// endregion IPCS debug messages


// region proc/threads debug msgs

#define PROCESS_RUN "RUNNING PROCESS %s..."
#define THREAD_RUN "RUNNING THREAD %s..."
#define PROCESS_EXIT "EXITING PROCESS %s..."
#define THREAD_EXIT "EXITING THREAD %s..."

// endregion proc/threads debug msgs

// endregion constants

// region macros

/**
 * @macro DEBUG_MSG
 * @brief Macro that calls the function debug_msg with the line number and the name of the file which it's called in.
 *
 * @param debug_msg
 * The debug message to be presented (might be formatted).
 *
 * @param __VA_ARGS__
 * The formatting arguments.
 *
 */
#define DEBUG_MSG(d_msg, deb_level, ...) if (DEBUG) debug_msg(__FILE__, __LINE__, d_msg, deb_level, __VA_ARGS__);

// endregion macros

// region public functions prototypes

/**
 * @def debug_msg
 * @brief Function that presents debugging messages in stdout.
 *
 * @param file_name
 * The name of the file from which this function is called.
 *
 * @param line
 * The line of the file from which this function is called.
 *
 * @param msg
 * The debug message to be presented (might be formatted).
 *
 * @param ...
 * The formatting arguments.
 *
 */
#define HERE(msg) printf("--------HERE--------- %s\n", msg);

extern void debug_msg(const char * file_name, int line, const char * msg, debug_level deb_level, ...);
extern void debug_init(debug_level deb_level, int sh_origin);

// endregion public functions prototypes

#endif //RACESIMULATOR_C_DEBUG_H