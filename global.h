#ifndef RACESIMULATOR_GLOBAL_H
#define RACESIMULATOR_GLOBAL_H

#include <semaphore.h>

#define true 1
#define false 0
#define DEBUG true

#define MUTEX "MUTEX"

typedef struct shared_memory shared_memory;
typedef struct race_config race_config;
typedef struct race_box race_box;

static int shm_id;
static shared_memory * mem_struct;
static sem_t * mutex;


#endif // RACESIMULATOR_GLOBAL_H