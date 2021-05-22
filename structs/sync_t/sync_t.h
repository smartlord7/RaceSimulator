/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
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

typedef struct sync_t sync_t;

/**
 * @struct sync_t
 * @brief Structure that contains all of the synchronization mechanisms and variables used on the race synchronization.
 *
 * @var sync_t::simulation_ended
 * Flag to check if the simulation is to end.
 *
 * @var sync_t::race_running
 * Flag to check if the race is in a running state.
 *
 * @var sync_t::race_interrupted
 * Flag to check if the race has been interrupted.
 *
 * @var sync_t::race_loop
 * Flag to check if the race has been interrupted but can be restarted later.
 *
 * @var sync_t::clock_on
 * Flag to check if the clock is active.
 *
 * @var sync_t::clock_paused
 * Flag to check if the clock is paused.
 *
 * @var sync_t::num_clock_waiters
 * Counter for the number of threads waiting for the clock.
 *
 * @var sync_t::global_time
 * Counter to the global time units.
 *
 * @var sync_t::access_mutex
 * Pthread mutex used in accesses to the shared memory region.
 *
 * @var sync_t::clock_rise_mutex
 * Pthread mutex used to control the clock state change to rise.
 *
 * @var sync_t::clock_valley_mutex
 * Pthread mutex used to control the clock state change to valley.
 *
 * @var sync_t::cond
 * Condition variable used to synchronize threads.
 *
 * @var sync_t::clock_rise_cond
 * Condition variable used to signal/broadcast that the clock is on a rise state.
 *
 * @var sync_t::clock_valley_cond
 * Condition variable used to signal/broadcast that the clock is on a valley state.
 *
 */
typedef struct sync_t {
    int simulation_ended, race_running, race_interrupted, race_loop, clock_on, clock_paused, num_clock_waiters, global_time;
    mutex_t access_mutex, clock_rise_mutex, clock_valley_mutex;
    cond_t cond, clock_rise_cond, clock_valley_cond;
} sync_t;

//endregion structures

#endif //RACESIMULATOR_C_SYNC_T_H
