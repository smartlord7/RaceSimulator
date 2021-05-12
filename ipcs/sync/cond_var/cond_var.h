#ifndef IPCS_SYNC_COND_VAR_H
#define IPCS_SYNC_COND_VAR_H

#include "pthread.h"
#include "../mutex/mutex.h"

typedef pthread_cond_t cond_t;

extern void wait_cond(cond_t * cond, mutex_t * mutex);
extern void init_cond(cond_t * cond, int proc_shared);
extern void destroy_cond(cond_t * cond);
extern void notify_cond_all(cond_t * cond);
extern void notify_cond(cond_t * cond);

#endif //IPCS_SYNC_COND_VAR_H
