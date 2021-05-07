#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "assert.h"
#include "pthread.h"
#include "../../../util/exception_handler/exception_handler.h"
#include "../mutex/mutex.h"
#include "monitor.h"


monitor_t ** monitors = NULL;
int monitor_i = 0;

void monitor_init(int * condit, cond_t * cond_var, mutex_t * cond_mutex, int shared_proc) {
    assert(condit != NULL && cond_var != NULL && cond_mutex != NULL);

    monitors = (monitor_t **) realloc(monitors, monitor_i * sizeof(monitor_t *));

    throw_if_exit(monitors, MEMORY_ALLOCATE_EXCEPTION, MONITOR);

    if (shared_proc) {
        set_shared_proc(monitor_i);
    }

    monitors[monitor_i]->condition = condit;
    monitors[monitor_i]->cond_var = cond_var;
    monitors[monitor_i]->mutex = cond_mutex;

    monitor_i++;
}

void set_condition(int * condit, int i) {
    assert(condit != NULL && i < monitor_i);

    monitors[i]->condition = condit;
}

void set_cond_var(cond_t * cond_var, int i) {
    assert(i < monitor_i);

    monitors[i]->cond_var = cond_var;
}

void set_mutex(mutex_t * cond_mutex, int i) {
    assert(cond_mutex != NULL && i < monitor_i);

    monitors[i]->mutex = cond_mutex;
}

void set_shared_proc(int i) {
    assert(i < monitor_i);

    pthread_condattr_t attr;

    throw_if_exit(pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0, COND_VAR_SHARE_EXCEPTION, "");
    set_sh_mutex(monitors[i]->mutex);
    monitors[i]->shared_proc = true;

    throw_if_exit(pthread_cond_init(monitors[i]->cond_var, &attr) != 0, COND_VAR_INITIALIZE_EXCEPTION, "");
}

void monitor_destroy_all() {
    int i = 0;

    while (i < monitor_i) {
        if (monitors[i] != NULL) {
            monitor_destroy(i);
        }
    }

    monitor_i = 0;
}

void monitor_destroy(int i) {
    assert(i < monitor_i);

    throw_if_exit(pthread_cond_destroy(monitors[i]->cond_var) != 0, COND_VAR_DESTROY_EXCEPTION, "");
    destroy_mutex(monitors[i]->mutex);

    free(&monitors[i]);

    monitors[i] = NULL;
}

void monitor_wait(int i) {
    assert(i < monitor_i);

    int err = pthread_cond_wait(monitors[i]->cond_var, monitors[i]->mutex);

    throw_if_exit(err != 0 && err != EINVAL, COND_VAR_WAIT_EXCEPTION, "");
}

void monitor_broadcast(int i) {
    assert(i < monitor_i);

    throw_if_exit(pthread_cond_broadcast(monitors[i]->cond_var) != 0, COND_VAR_BROADCAST_EXCEPTION, "");
}

void change_condition(int new_value, int i) {
    lock_mutex(monitors[i]->mutex);

    * monitors[i]->condition = new_value;
    monitor_broadcast(i);

    unlock_mutex(monitors[i]->mutex);
}

void wait_condition_change(int i) {
    lock_mutex(monitors[i]->mutex);

    while (! (* monitors[i]->condition)) {
        monitor_wait(i);
    }

    unlock_mutex(monitors[i]->mutex);
}

