/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACESIMULATOR_C_PROCESS_MANAGER_H
#define RACESIMULATOR_C_PROCESS_MANAGER_H

// region dependencies

#include <pthread.h>

// endregion dependencies

// region constants

#define true 1
#define false 0
#define MAX_CMD_SIZE 20

// endregion constants

// region public functions prototypes

/**
 * @def create_process
 * @brief Function that creates a process via fork().
 *
 * @param proc_name
 * The name of the process.
 *
 * @param worker
 * The function that will be called when the process is created.
 *
 * @param params
 * The parameters of the worker function.
 *
 * @throws ProcessCreationException if the process can't be created.
 */
void create_process(const char * proc_name, void (* worker)(void *), void * params);

/**
 * @def wait_procs
 * @brief Function that waits for all the child processes of the current process.
 *
 */
void wait_procs();

/**
 * @def terminate_proc_grp
 * @brief Function that terminates all the processes within the process group with the specified id.
 *
 * @param proc_group_id
 * The process group id.
 */
void terminate_proc_grp(pid_t proc_group_id);

/**
 * @def create_thread
 * @brief Function that creates a thread via pthread_create().
 *
 * @param thread_name
 * The name of the thread.
 *
 * @param thread_p
 * A pointer to a pthread_t.
 *
 * @param worker
 * The function that will be called when the thread is created.
 *
 * @param params
 * The parameters of the worker function.
 *
 * @throws ThreadCreationException if the thread can't be created.
 *
 */

void create_thread(const char * thread_name, pthread_t * thread_p, void * (* worker)(void *), void * params);

/**
 * @def wait_threads
 * @brief Function that waits for and kills an array of threads.
 *
 * @param num_threads
 * The number of threads to be waited for and killed.
 *
 * @param threads
 * A pointer to the threads array.
 *
 * @throws ThreadJoinException if a thread can't be waited for.
 *
 */
void wait_threads(int num_threads, pthread_t * threads);

// endregion public functions prototypes

#endif //RACESIMULATOR_C_PROCESS_MANAGER_H