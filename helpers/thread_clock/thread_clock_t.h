/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef RACE_HELPERS_THREAD_CLOCK_H
#define RACE_HELPERS_THREAD_CLOCK_H

// region dependencies

#include "../../ipcs/sync/mutex/mutex.h"
#include "../../ipcs/sync/cond_var/cond_var.h"

// endregion dependencies

// region structures

typedef struct thread_clock_t thread_clock_t;

/**
 * @struct thread_clock
 * @brief Structure that contains the data needed to asynchronously synchronize the threads :-).
 *
 * @var thread_clock::clock_on
 * Flag that specifies if the clock is active or not.
 *
 * @var thread_clock::clock_paused
 * Flag to check if the clock is paused.
 *
 * @var thread_clock::num_clock_waiters
 * The current number of threads waiting for the clock.
 *
 * @var thread_clock::global_time
 * Counter that indicates the global time in time units.
 *
 * @var thread_clock::clock_rise_mutex
 * pthread mutex used to control the clock state change to rise.
 *
 * @var thread_clock::clock_valley_mutex
 * pthread mutex used to control the clock state change to valley.
 *
 * @var thread_clock::cond
 * Condition variable used to synchronize the clock usage.
 *
 * @var thread_clock::clock_rise_cond
 * Condition variable used to signal/broadcast that the clock is on a rise state or that the threads should check their sync_sleep loop conditions.
 *
 * @var thread_clock::clock_valley_cond
 * Condition variable used to signal/broadcast that the clock is on a valley state or that it should check its loop conditions.
 *
 */
typedef struct thread_clock_t {
    int clock_on, clock_paused, num_clock_waiters, global_time;
    mutex_t clock_rise_mutex, clock_valley_mutex;
    cond_t clock_rise_cond, clock_valley_cond;
} thread_clock_t;

//endregion structures

// region public function prototypes

/**
 * @def init_global_clock
 * @brief Function that initializes the internal global clock.
 */
extern void init_global_clock();

/**
 * @def sync_sleep
 * @brief Function that allows the synchronization of a set threads trying to enter a sleep state.
 *
 * @param time_units
 * Duration of the sleep in time units.
 *
 */
extern void sync_sleep(int time_units);


/**
 * @def pause_and_restart_clock
 * @brief Function that pauses and restarts the clock global time.
 */
extern void pause_and_restart_clock();

/**
 * @def unpause_clock
 * @brief Function that unpauses the clock.
 */
extern void unpause_clock();

/**
 * @def end_clock
 * @brief Function that signals the clock to end.
 */
extern void end_clock();

// endregion public function prototypes.

#endif //RACE_HELPERS_THREAD_CLOCK_H
