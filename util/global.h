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

#include <semaphore.h>
#include "debug/debug.h"
#include "../structs/shared_memory/shared_memory_t.h"

// endregion dependencies

// region constants

#define true 1
#define false 0
#define OUTPUT_MUTEX "OUTPUT_MUTEX"
#define SHM_MUTEX "SHM_MUTEX"
#define RACE_START_COND_VAR "RACE_START_COND_VAR"

#define SMALLEST_SIZE 16
#define XSMALL_SIZE 32
#define SMALL_SIZE 64
#define MEDIUM_SIZE 128
#define LARGE_SIZE 256
#define XLARGE_SIZE 512
#define LARGEST SIZE 1024

// endregion constants

// region global variables

extern int shm_id;
extern shared_memory_t * shm;
extern sem_t * output_mutex, * shm_mutex, ** boxes_availability;

// endregion global variables

#endif // RACESIMULATOR_C_GLOBAL_H