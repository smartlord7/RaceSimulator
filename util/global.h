<<<<<<< Updated upstream
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
#define SHM_MUTEX "SHM_MUTEX"
#define RACE_START_COND_VAR "RACE_START_COND_VAR"
#define THREAD_MUTEX "THREAD_MUTEX"
#define RACE_START_MONITOR 0
#define GLOBAL_TIMER_START 1
#define GLOBAL_TIMER_END 2

#define SYNC lock_mutex(&shm->sync_s.start_mutex);
#define END_SYNC unlock_mutex(&shm->sync_s.start_mutex);
#define SYNC_CAR lock_mutex(&car->mutex);
#define END_SYNC_CAR unlock_mutex(&car->mutex);
#define tu_to_usec(t) (unsigned int) ((t) / config.time_units_per_sec * pow(10, 6))

#define SMALLEST_SIZE 16
#define XSMALL_SIZE 32
#define SMALL_SIZE 64
#define MEDIUM_SIZE 128
#define LARGE_SIZE 256
#define XLARGE_SIZE 512
#define LARGEST_SIZE 1024

// endregion constants

// region global variables

extern int shm_id, malfunction_msg_q_id, fd_named_pipe;
extern shared_memory_t * shm;
extern race_config_t config;
extern sem_t ** boxes_availability;

// endregion global variables

=======
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
#define SHM_MUTEX "SHM_MUTEX"
#define RACE_START_COND_VAR "RACE_START_COND_VAR"
#define THREAD_MUTEX "THREAD_MUTEX"
#define RACE_START_MONITOR 0
#define GLOBAL_TIMER_START 1
#define GLOBAL_TIMER_END 2

#define SYNC lock_mutex(&shm->sync_s.start_mutex);
#define END_SYNC unlock_mutex(&shm->sync_s.start_mutex);
#define SYNC_CAR lock_mutex(&car->mutex);
#define END_SYNC_CAR unlock_mutex(&car->mutex);
#define tu_to_usec(t) (unsigned int) ((t) / config.time_units_per_sec * pow(10, 6))

#define SMALLEST_SIZE 16
#define XSMALL_SIZE 32
#define SMALL_SIZE 64
#define MEDIUM_SIZE 128
#define LARGE_SIZE 256
#define XLARGE_SIZE 512
#define LARGEST_SIZE 1024

// endregion constants

// region global variables

extern int shm_id, malfunction_msg_q_id, fd_named_pipe;
extern shared_memory_t * shm;
extern race_config_t config;
extern sem_t ** boxes_availability;

// endregion global variables

>>>>>>> Stashed changes
#endif // RACESIMULATOR_C_GLOBAL_H