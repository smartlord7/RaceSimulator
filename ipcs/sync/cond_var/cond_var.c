/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 18/05/2021
*/

#include "assert.h"
#include "pthread.h"
#include "cond_var.h"
#include "../../../util/exception_handler/exception_handler.h"

/**
 * @def wait_cond
 * @brief Function that puts a condition variable in a waiting state.
 *
 * @param cond
 * Condition variable to be put on wait.
 *
 * @param mutex
 * Pthread mutex associated with condition variable.
 *
 * @throws CondVarWaitException if it is not possible to put variable on wait.
 *
 */
void wait_cond(cond_t * cond, mutex_t * mutex) {
    assert(cond != NULL && mutex != NULL);

    throw_if_exit(pthread_cond_wait(cond, mutex) != 0, COND_VAR_WAIT_EXCEPTION, "");
}

/**
 * @def init_cond
 * @brief Function that initializes a condition variable.
 *
 * @param cond
 * Condition variable to be initialized.
 *
 * @param proc_shared
 * Flag to specify if the condition variable is to be shared between processes.
 *
 * @throws CondVarShareException if i
 *         CondVarInitializeException if it is not possible to initialize the variable.
 *
 */
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

/**
 * @def
 * @brief
 *
 * @param cond
 *
 * @throws
 *
 */
void destroy_cond(cond_t * cond) {
    assert(cond != NULL);

    throw_if_exit(pthread_cond_destroy(cond) != 0, COND_VAR_DESTROY_EXCEPTION, "");
}

/**
 * @def
 * @brief
 *
 * @param cond
 *
 * @throws
 *
 */
void notify_cond_all(cond_t * cond) {
    assert(cond != NULL);

    throw_if_exit(pthread_cond_broadcast(cond) != 0, COND_VAR_BROADCAST_EXCEPTION, "");
}

/**
 * @def
 * @brief
 *
 * @param cond
 *
 * @throws
 *
 */
void notify_cond(cond_t * cond) {
    assert(cond != NULL);

    throw_if_exit(pthread_cond_signal(cond) != 0, COND_VAR_SIGNAL_EXCEPTION, "");
}