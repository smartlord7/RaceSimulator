#ifndef RACESIMULATOR_C_SYNC_T_H
#define RACESIMULATOR_C_SYNC_T_H

#include <pthread.h>

//TODO Documentation

typedef struct sync_t sync_t;

struct sync_t {
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;
    pthread_cond_t cond;
};

#endif //RACESIMULATOR_C_SYNC_T_H
