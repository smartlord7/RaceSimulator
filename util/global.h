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

#define SYNC_CLOCK_RISE lock_mutex(&shm->sync_s.clock_rise_mutex);
#define END_SYNC_CLOCK_RISE unlock_mutex(&shm->sync_s.clock_rise_mutex);
#define SYNC_CLOCK_VALLEY lock_mutex(&shm->sync_s.clock_valley_mutex);
#define END_SYNC_CLOCK_VALLEY unlock_mutex(&shm->sync_s.clock_valley_mutex);
#define SYNC lock_mutex(&shm->sync_s.access_mutex);
#define END_SYNC unlock_mutex(&shm->sync_s.access_mutex);
#define SYNC lock_mutex(&shm->sync_s.access_mutex);
#define END_SYNC unlock_mutex(&shm->sync_s.access_mutex);
#define SYNC_CAR lock_mutex(&car->access_mutex);
#define END_SYNC_CAR unlock_mutex(&car->access_mutex);
#define SYNC_CAR_COND lock_mutex(&car->cond_mutex);
#define END_SYNC_CAR_COND unlock_mutex(&car->cond_mutex);
#define SYNC_TEAM lock_mutex(&team->access_mutex);
#define END_SYNC_TEAM unlock_mutex(&team->access_mutex);
#define SYNC_BOX lock_mutex(&box->access_mutex);
#define END_SYNC_BOX unlock_mutex(&box->access_mutex);
#define SYNC_BOX_COND lock_mutex(&box->cond_mutex);
#define END_SYNC_BOX_COND unlock_mutex(&box->cond_mutex);

#define RACE_SIMULATOR_NAMED_PIPE "RACE_SIMULATOR_NAMED_PIPE"
#define NAMED_PIPE_INDEX 0
#define GLOBAL_CLOCK_SAYS "[GLOBAL_CLOCK] "
#define GLOBAL_CLOCK_TIME GLOBAL_CLOCK_SAYS "%d tu"
#define GLOBAL_CLOCK_WAITERS GLOBAL_CLOCK_SAYS "WAITERS: %d"
#define GLOBAL_CLOCK_START GLOBAL_CLOCK_SAYS "STARTED..."
#define GLOBAL_CLOCK_RECEIVED GLOBAL_CLOCK_SAYS "RECEIVED %d THREADS, NEEDING %d MORE!"
#define GLOBAL_CLOCK_READY GLOBAL_CLOCK_SAYS "ALL THREADS RECEIVED!"
#define GLOBAL_CLOCK_VALLEY GLOBAL_CLOCK_SAYS "CLOCK VALLEY!"
#define GLOBAL_CLOCK_RISE GLOBAL_CLOCK_SAYS "CLOCK RISE!"
#define GLOBAL_CLOCK_RELEASE GLOBAL_CLOCK_SAYS "RELEASE ALL THREADS!"

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
extern char * mmap;

// endregion global variables

// region global variables

extern void sync_sleep(int time_units);
extern void wait_race_start();

// endregion global variables

#endif // RACESIMULATOR_C_GLOBAL_H