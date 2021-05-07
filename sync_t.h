#ifndef RACESIMULATOR_C_SYNC_T_H
#define RACESIMULATOR_C_SYNC_T_H

#include <pthread.h>
#include "ipcs/sync/mutex/mutex.h"
#include "ipcs/sync/monitor/monitor.h"

//TODO Documentation

typedef struct sync_t sync_t;

struct sync_t {
    int start, global_timer_start, global_timer_end;
    mutex_t start_mutex, global_timer_start_mutex, global_timer_end_mutex;
    cond_t start_cond, global_timer_start_cond, global_timer_end_cond;
};

#endif //RACESIMULATOR_C_SYNC_T_H
