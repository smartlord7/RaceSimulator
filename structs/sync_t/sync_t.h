#ifndef RACESIMULATOR_C_SYNC_T_H
#define RACESIMULATOR_C_SYNC_T_H

#include <pthread.h>
#include "../../ipcs/sync/mutex/mutex.h"
#include "../../ipcs/sync/cond_var/cond_var.h"

//TODO Documentation

typedef struct sync_t sync_t;

typedef struct sync_t {
    int race_running, race_interrupted, race_loop, clock_on, clock_paused, num_clock_waiters, global_time;
    mutex_t access_mutex, clock_rise_mutex, clock_valley_mutex;
    cond_t cond, clock_rise_cond, clock_valley_cond;
} sync_t;

#endif //RACESIMULATOR_C_SYNC_T_H
