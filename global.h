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
#include "util/debug.h"
#include "structs/shared_memory_t.h"

// endregion dependencies

// region constants

#define true 1
#define false 0
#define DEBUG false
#define OUTPUT_MUTEX "OUTPUT_MUTEX"
#define SHM_MUTEX "SHM_MUTEX"
#define RACE_START "RACE_START"
#define MALFUNCTION_MNG_START "MALFUNCTION_MNG_START"

// endregion constants

// region global variables

extern int shm_id;
extern shared_memory_t * mem_struct;
extern sem_t * output_mutex, * shm_mutex, * race_start, * malfunction_mng_start, ** boxes_availability;

// endregion global variables

#endif // RACESIMULATOR_C_GLOBAL_H