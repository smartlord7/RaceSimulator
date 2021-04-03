#ifndef RACESIMULATOR_GLOBAL_H
#define RACESIMULATOR_GLOBAL_H

#include <semaphore.h>
#include "structs/shared_memory.h"

#define MAX_BUFFER_SIZE 512
#define MAX_LABEL_SIZE 50

#define true 1
#define false 0
#define DEBUG true

#define RACE_MANAGER "RACE_MANAGER"
#define MALFUNCTION_MANAGER "MALFUNCTION_MANAGER"
#define TEAM_MANAGER "TEAM_MANAGER"
#define RACE_TEAM "RACE_TEAM"
#define RACE_CAR "RACE_CAR"
#define CAR_THREAD "CAR_THREAD"

#define MUTEX "MUTEX"
#define RACE_START "RACE_START"
#define BOX_SEM_PREFIX "BOX_"

int shm_id;
shared_memory * mem_struct;
sem_t * mutex, * race_start, ** boxes_availability;

void race_manager(void * data);
void malfunction_manager(void * data);
void team_manager(void * data);

#endif // RACESIMULATOR_GLOBAL_H