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

// region ipcs debug msgs
#define SHM_CREATED "CREATED SHARED MEMORY WITH ID %d!"
#define SHM_ATTACHED "ATTACHED SHARED MEMORY WITH ID %d TO CURRENT PROCESS ADDRESSING ZONE!"
#define SEM_CREATED "CREATED SEMAPHORE NAMED %s!"
#define SHM_DETACHED "DETACHED SHARED MEMORY WITH ID %d!"
#define SHM_REMOVED "REMOVED SHARED MEMORY WITH ID %d!"
#define SEM_CLOSED "CLOSED SEMAPHORE NAMED %s!"
#define SEM_UNLINKED "UNLINKED SEMAPHORE NAMED %s!"
#define IPCS_CREATED "CREATED IPCS!"

// endregion IPCS debug messages

// region proc/threads debug msgs

#define RACE_CONFIG_CREATED "CREATED RACE CONFIGURATION!"
#define RUNNING_PROCESS "RUNNING PROCESS %s..."
#define RUNNING_THREAD "RUNNING THREAD %s..."
#define EXITING_PROCESS "EXITING PROCESS %s..."
#define EXITING_THREAD "EXITING THREAD %s..."

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
#define DEBUG_MSG(d_msg, ...) if (DEBUG) debug_msg(__FILE__, __LINE__, d_msg, __VA_ARGS__);

// endregion macros

// region global variables

extern sem_t * deb_mutex;

// endregion global variables

// region public functions prototypes

/**
 * @def debug_init
 * @brief Function that initializes the debugging mechanisms with a mutex semaphore if synchronization across processes/threads is needed.
 *
 * @param mutex
 * The mutex that provides synchronized access to stdout.
 *
 */
void debug_init(sem_t * mutex);

/**
 * @def debug_msg
 * @brief Function that presents debugging messages in stdout.
 *
 * @param file
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
void debug_msg(const char * file_name, int line, const char * msg, ...);

// endregion public functions prototypes

#endif //RACESIMULATOR_C_DEBUG_H