#ifndef IPCS_COND_VAR_H
#define IPCS_COND_VAR_H

#include "../mutex/mutex.h"

#define true 1
#define false 0
#define MONITOR "MONITOR"

typedef pthread_cond_t cond_t;
typedef struct monitor_t monitor_t;

extern void monitor_init(int * condit, cond_t * cond_var, mutex_t * cond_mutex, int shared_proc);
extern void monitor_destroy_all();
extern void monitor_destroy(int i);
extern void set_shared_proc(int i);
extern void monitor_wait(int i);
extern void change_condition(int new_value, int i);
extern void monitor_broadcast(int i);
extern void wait_condition_change(int i);
extern void set_condition(int * condit, int i);
extern void set_cond_var(cond_t * cond_var, int i);
extern void set_mutex(mutex_t * cond_mutex, int i);

struct monitor_t {
    int shared_proc, * condition;
    cond_t * cond_var;
    mutex_t * mutex;
};

#endif //IPCS_COND_VAR_H
