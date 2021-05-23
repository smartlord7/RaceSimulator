/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef RACESIMULATOR_C_SYNC_T_H
#define RACESIMULATOR_C_SYNC_T_H

//region dependencies

#include <pthread.h>
#include "../../ipcs/sync/mutex/mutex.h"
#include "../../ipcs/sync/cond_var/cond_var.h"

//endregion dependencies

//region structures

typedef struct thread_clock_t thread_clock_t;

/**
 * @struct thread_clock
 * @brief Structure that contains all of the synchronization mechanisms and variables used on the race synchronization.
 *
 * @var thread_clock::simulation_ended
 * Flag to check if the simulation is to end.
 *
 * @var thread_clock::race_running
 * Flag to check if the race is in a running state.
 *
 * @var thread_clock::race_interrupted
 * Flag to check if the race has been interrupted.
 *
 * @var thread_clock::race_loop
 * Flag to check if the race has been interrupted but can be restarted later.
 *
 * @var thread_clock::clock_on
 * Flag to check if the clock is active.
 *
 * @var thread_clock::clock_paused
 * Flag to check if the clock is paused.
 *
 * @var thread_clock::num_clock_waiters
 * Counter for the number of threads waiting for the clock.
 *
 * @var thread_clock::global_time
 * Counter to the global time units.
 *
 * @var thread_clock::access_mutex
 * Pthread mutex used in accesses to the shared memory region.
 *
 * @var thread_clock::clock_rise_mutex
 * Pthread mutex used to control the clock state change to rise.
 *
 * @var thread_clock::clock_valley_mutex
 * Pthread mutex used to control the clock state change to valley.
 *
 * @var thread_clock::cond
 * Condition variable used to synchronize threads.
 *
 * @var thread_clock::clock_rise_cond
 * Condition variable used to signal/broadcast that the clock is on a rise state.
 *
 * @var thread_clock::clock_valley_cond
 * Condition variable used to signal/broadcast that the clock is on a valley state.
 *
 */
typedef struct thread_clock_t {
    int clock_on, clock_paused, num_clock_waiters, global_time;
    mutex_t clock_rise_mutex, clock_valley_mutex;
    cond_t clock_rise_cond, clock_valley_cond;
} thread_clock_t;

//endregion structures

#endif //RACESIMULATOR_C_SYNC_T_H
