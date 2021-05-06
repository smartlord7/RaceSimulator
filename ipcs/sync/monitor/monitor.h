#ifndef IPCS_COND_VAR_H
#define IPCS_COND_VAR_H

#include "../mutex/mutex.h"

#define true 1
#define false 0

typedef pthread_cond_t cond_t;

extern void monitor_init(int * condit, cond_t * cond_var, mutex_t * cond_mutex);
extern void monitor_destroy();
extern void set_shared_proc();
extern void monitor_wait();
extern void change_condition(int new_value);
extern void monitor_broadcast();
extern void wait_condition_change();
extern void set_condition(int * condit);
void set_cond_var(cond_t * cond_var);
void set_mutex(mutex_t * cond_mutex);


#endif //IPCS_COND_VAR_H
