#include "assert.h"
#include "errno.h"
#include "pthread.h"
#include "../../../util/exception_handler/exception_handler.h"
#include "mutex.h"

void set_sh_mutex(mutex_t * mutex) {
    assert(mutex != NULL);

    pthread_mutexattr_t attr;

    throw_if_exit(pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0, MUTEX_SHARE_EXCEPTION, "");

    throw_if_exit(pthread_mutex_init(mutex, &attr) != 0, MUTEX_INITIALIZE_EXCEPTION, "");
}

void destroy_mutex(mutex_t * mutex) {
    assert(mutex != NULL);

<<<<<<< Updated upstream
    throw_if_exit(pthread_mutex_destroy(mutex) != 0, MUTEX_CLOSE_EXCEPTION, "");
}

void lock_mutex(mutex_t * mutex) {
    assert(mutex != NULL);

    int ret = pthread_mutex_lock(mutex);

    throw_if_exit(ret != 0 && ret != EINVAL, MUTEX_LOCK_EXCEPTION, "");
}

void unlock_mutex(mutex_t * mutex) {
    assert(mutex != NULL);

    int ret = pthread_mutex_lock(mutex);

    throw_if_exit(ret != 0 && ret != EINVAL, MUTEX_UNLOCK_EXCEPTION, "");
}
=======
    if (pthread_mutex_destroy(mutex) != 0) {
        throw_and_exit(MUTEX_CLOSE_EXCEPTION, "");
    }
}
>>>>>>> Stashed changes
