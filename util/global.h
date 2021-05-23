/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACESIMULATOR_C_GLOBAL_H
#define RACESIMULATOR_C_GLOBAL_H

// region dependencies

#include "math.h"
#include "semaphore.h"
#include "debug/debug.h"
#include "../structs/shared_memory/shared_memory_t.h"

// endregion dependencies

// region constants

#define true 1
#define false 0

#define SYNC_CLOCK_RISE lock_mutex(&shm->thread_clock.clock_rise_mutex);
#define END_SYNC_CLOCK_RISE unlock_mutex(&shm->thread_clock.clock_rise_mutex);
#define SYNC_CLOCK_VALLEY lock_mutex(&shm->thread_clock.clock_valley_mutex);
#define END_SYNC_CLOCK_VALLEY unlock_mutex(&shm->thread_clock.clock_valley_mutex);
#define SYNC lock_mutex(&shm->mutex);
#define END_SYNC unlock_mutex(&shm->mutex);
#define SYNC_CAR lock_mutex(&car->access_mutex);
#define END_SYNC_CAR unlock_mutex(&car->access_mutex);
#define SYNC_CAR_COND lock_mutex(&car->cond_mutex);
#define END_SYNC_CAR_COND unlock_mutex(&car->cond_mutex);
#define SYNC_BOX_COND lock_mutex(&box->mutex);
#define END_SYNC_BOX_COND unlock_mutex(&box->mutex);

#define GLOBAL_CLOCK "[GLOBAL_CLOCK_SAYS] "
#define GLOBAL_CLOCK_TIME GLOBAL_CLOCK "%d tu"
#define GLOBAL_CLOCK_WAITERS GLOBAL_CLOCK "WAITERS: %d"
#define GLOBAL_CLOCK_START GLOBAL_CLOCK "STARTED..."
#define GLOBAL_CLOCK_RECEIVED GLOBAL_CLOCK "RECEIVED %d THREADS, NEEDING %d MORE!"
#define GLOBAL_CLOCK_READY GLOBAL_CLOCK "ALL THREADS RECEIVED!"
#define GLOBAL_CLOCK_VALLEY GLOBAL_CLOCK "CLOCK VALLEY!"
#define GLOBAL_CLOCK_RISE GLOBAL_CLOCK "CLOCK RISE!"
#define GLOBAL_CLOCK_RELEASE GLOBAL_CLOCK "RELEASE ALL THREADS!"

#define RACE_SIMULATOR_PROCESS "RACE_SIMULATOR"
#define RACE_SIMULATOR_NAMED_PIPE "RACE_SIMULATOR_NAMED_PIPE"
#define NAMED_PIPE_INDEX 0
#define SIGNAL_SIGINT "SIGINT"
#define SIGNAL_SIGSEGV "SIGSEGV"
#define SIGNAL_SIGTSTP "SIGTSTP"
#define SIGNAL_SIGUSR1 "SIGUSR1"

#define SMALLEST_SIZE 16
#define XSMALL_SIZE 32
#define SMALL_SIZE 64
#define MEDIUM_SIZE 128
#define LARGE_SIZE 256
#define XLARGE_SIZE 512
#define LARGEST_SIZE 1024

// endregion constants

// region global variables

extern int shm_id, malfunction_q_id;
extern shared_memory_t * shm;
extern race_config_t config;
extern char * mmap_f;

// endregion global variables

// region global variables

/**
 * @def sync_sleep
 * @brief Function that allows the synchronization of a thread trying to enter a sleep state.
 *
 * @param time_units
 * Duration of the sleep in time units.
 *
 */
extern void sync_sleep(int time_units);

/**
 * @def wait_race_start
 * @brief Function that allows to wait for a signal about the race start and if it has began.
 *
 * @return false if the race has not began.
 *         true if the race has began.
 */
extern int wait_race_start();


extern void signal_handler(int signum);

/**
 * @def pause_and_restart_clock
 * @brief Function that pauses and restarts the clock.
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

extern void notify_race_state_change();

// endregion global variables

#endif // RACESIMULATOR_C_GLOBAL_H