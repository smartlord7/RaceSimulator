#ifndef RACESIMULATOR_GLOBAL_H
#define RACESIMULATOR_GLOBAL_H

#include <semaphore.h>
#include "util/debug.h"
#include "structs/shared_memory.h"

#define MAX_BUFFER_SIZE 512
#define MAX_LABEL_SIZE 50

#define true 1
#define false 0
#define DEBUG true

#define RACE_SIMULATOR "RACE_SIMULATOR"
#define RACE_MANAGER "RACE_MANAGER"
#define MALFUNCTION_MANAGER "MALFUNCTION_MANAGER"
#define TEAM_MANAGER "TEAM_MANAGER"
#define RACE_TEAM "RACE_TEAM"
#define RACE_CAR "RACE_CAR"
#define CAR_THREAD "CAR_THREAD"

#define OUTPUT_MUTEX "OUTPUT_MUTEX"
#define SHM_MUTEX "SHM_MUTEX"
#define RACE_START "RACE_START"
#define MALFUNCTION_MNG_START "MALFUNCTION_MNG_START"
#define BOX_SEM_PREFIX "BOX_"

#define S_DEBUG_MSG(msg, ...) SYNC_DEBUG_MSG(output_mutex, msg, __VA_ARGS__)
#define s_throw_exception_end_exit(msg, ...) sync_throw_exception_end_exit(output_mutex, msg, __VA_ARGS__)
#define s_throw_exception_end_stay(msg, ...) sync_throw_exception_end_stay(output_mutex, msg, __VA_ARGS__)

int shm_id;
shared_memory * mem_struct;
sem_t * output_mutex, * shm_mutex, * race_start, * malfunction_mng_start, ** boxes_availability;

void race_manager();
void malfunction_manager();
void team_manager(void * data);

#endif // RACESIMULATOR_GLOBAL_H