/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#include <stdio.h>
#include <errno.h>
#include "assert.h"
#include "pthread.h"
#include "../../../util/exception_handler/exception_handler.h"
#include "mutex.h"

void init_mutex(mutex_t * mutex, int proc_shared) {
    if (proc_shared) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        throw_if_exit(pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0, MUTEX_SHARE_EXCEPTION, "");
        throw_if_exit(pthread_mutex_init(mutex, &attr) != 0, MUTEX_INITIALIZE_EXCEPTION, "");
    } else {
        throw_if_exit(pthread_mutex_init(mutex, NULL) != 0, MUTEX_INITIALIZE_EXCEPTION, "");
    }
}

void destroy_mutex(mutex_t * mutex) {
    assert(mutex != NULL);

    unlock_mutex(mutex);
    int ret = pthread_mutex_destroy(mutex);

    throw_if_exit(ret != 0 && ret != EBUSY, MUTEX_CLOSE_EXCEPTION, "");
}

void lock_mutex(mutex_t * mutex) {
    assert(mutex != NULL);

    throw_if_exit(pthread_mutex_lock(mutex) != 0, MUTEX_LOCK_EXCEPTION, "");
}

void unlock_mutex(mutex_t * mutex) {
    assert(mutex != NULL);

    throw_if_exit(pthread_mutex_unlock(mutex) != 0, MUTEX_UNLOCK_EXCEPTION, "");
}

