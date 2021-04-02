#ifndef RACESIMULATOR_GLOBAL_H
#define RACESIMULATOR_GLOBAL_H

#include <semaphore.h>
#include "structs/shared_memory.h"

#define MAX_BUFFER_SIZE 512
#define MAX_TEAM_LABEL_SIZE 10

#define true 1
#define false 0
#define DEBUG true

#define RACE_MANAGER "RACE_MANAGER"
#define TEAM_MANAGER "TEAM_MANAGER"
#define MALFUNCTION_MANAGER "MALFUNCTION_MANAGER"
#define RACE_CAR "RACE_CAR"
#define RACE_TEAM "RACE_TEAM"

#define MUTEX "MUTEX"

static int shm_id;
static shared_memory * mem_struct;
static sem_t * mutex;

void race_manager(void * data);
void malfunction_manager(void * data);
void team_manager(void * data);



#endif // RACESIMULATOR_GLOBAL_H