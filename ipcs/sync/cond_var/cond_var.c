/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 18/05/2021
*/

// region dependencies

#include "assert.h"
#include "pthread.h"
#include "cond_var.h"
#include "../../../util/exception_handler/exception_handler.h"

// endregion dependencies

// region public functions

void wait_cond(cond_t * cond, mutex_t * mutex) {
    assert(cond != NULL && mutex != NULL);

    throw_if_exit(pthread_cond_wait(cond, mutex) != 0, COND_VAR_WAIT_EXCEPTION, "");
}

void init_cond(cond_t * cond, int proc_shared) {

    if (proc_shared) {
        pthread_condattr_t attr;
        pthread_condattr_init(&attr);
        throw_if_exit(pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0, COND_VAR_SHARE_EXCEPTION, "");
        throw_if_exit(pthread_cond_init(cond, &attr) != 0, COND_VAR_INITIALIZE_EXCEPTION, "");
    } else {
        throw_if_exit(pthread_cond_init(cond, NULL) != 0, COND_VAR_INITIALIZE_EXCEPTION, "");
    }

}

void destroy_cond(cond_t * cond) {
    assert(cond != NULL);

    throw_if_exit(pthread_cond_destroy(cond) != 0, COND_VAR_DESTROY_EXCEPTION, "");
}

void notify_cond_all(cond_t * cond) {
    assert(cond != NULL);

    throw_if_exit(pthread_cond_broadcast(cond) != 0, COND_VAR_BROADCAST_EXCEPTION, "");
}

void notify_cond(cond_t * cond) {
    assert(cond != NULL);

    throw_if_exit(pthread_cond_signal(cond) != 0, COND_VAR_SIGNAL_EXCEPTION, "");
}

// endregion public functions