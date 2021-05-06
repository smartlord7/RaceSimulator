#include "stdio.h"
#include "errno.h"
#include "assert.h"
#include "pthread.h"
#include "../../../util/exception_handler/exception_handler.h"
#include "../mutex/mutex.h"
#include "monitor.h"

static cond_t * cond = NULL;
static mutex_t * mutex = NULL;
static int * condition = NULL;

void monitor_init(int * condit, cond_t * cond_var, mutex_t * cond_mutex) {
    assert(condit != NULL && cond_var != NULL && cond_mutex != NULL);

    condition = condit;
    cond = cond_var;
    mutex = cond_mutex;
}

void set_condition(int * condit) {
    assert(condit != NULL);

    condition = condit;
}

void set_cond_var(cond_t * cond_var) {
    assert(cond_var != NULL);

    cond = cond_var;
}

void set_mutex(mutex_t * cond_mutex) {
    assert(cond_mutex != NULL);

    mutex = cond_mutex;
}

void set_shared_proc() {
    assert(cond != NULL);

    pthread_condattr_t attr;

    throw_if_exit(pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0, COND_VAR_SHARE_EXCEPTION, "");

    throw_if_exit(pthread_cond_init(cond, &attr) != 0, COND_VAR_INITIALIZE_EXCEPTION, "");
}

void monitor_destroy() {
    assert(cond != NULL);

    throw_if_exit(pthread_cond_destroy(cond) != 0, COND_VAR_DESTROY_EXCEPTION, "");

    cond = NULL;
    mutex = NULL;
}

void monitor_wait() {
    assert(cond != NULL);

    int err = pthread_cond_wait(cond, mutex);

    throw_if_exit(err != 0 && err != EINVAL, COND_VAR_WAIT_EXCEPTION, "");
}

void monitor_broadcast() {
    assert(cond != NULL);

    throw_if_exit(pthread_cond_broadcast(cond) != 0, COND_VAR_BROADCAST_EXCEPTION, "");
}

void change_condition(int new_value) {
    lock_mutex(mutex);

    * condition = new_value;
    monitor_broadcast();

    unlock_mutex(mutex);
}

void wait_condition_change() {
    lock_mutex(mutex);

    while (! (* condition)) {
        monitor_wait();
    }

    unlock_mutex(mutex);
}

