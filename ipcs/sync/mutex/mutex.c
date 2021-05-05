#include "assert.h"
#include "pthread.h"
#include "../../../util/debug/exception_handler.h"
#include "mutex.h"

void set_sh_mutex(pthread_mutex_t * mutex) {
    assert(mutex != NULL);

    pthread_mutexattr_t attr;

    if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0) {
        throw_and_exit(MUTEX_SHARE_EXCEPTION, "");
    }

    if (pthread_mutex_init(mutex, &attr) != 0) {
        throw_and_exit(MUTEX_INITIALIZE_EXCEPTION, "");
    }
}

void destroy_mutex(pthread_mutex_t * mutex) {
    assert(mutex != NULL);

    if (pthread_mutex_destroy(mutex) != 0) {
        throw_and_exit(MUTEX_CLOSE_EXCEPTION, "");
    }
}
