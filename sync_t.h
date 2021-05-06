#ifndef RACESIMULATOR_C_SYNC_T_H
#define RACESIMULATOR_C_SYNC_T_H

#include <pthread.h>
#include "ipcs/sync/mutex/mutex.h"
#include "ipcs/sync/monitor/monitor.h"

//TODO Documentation

typedef struct sync_t sync_t;

struct sync_t {
    int start;
    mutex_t mutex;
    cond_t cond;
};

#endif //RACESIMULATOR_C_SYNC_T_H
