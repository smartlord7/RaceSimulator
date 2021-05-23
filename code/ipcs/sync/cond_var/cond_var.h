/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 18/05/2021
*/

#ifndef IPCS_SYNC_COND_VAR_H
#define IPCS_SYNC_COND_VAR_H

#include "pthread.h"
#include "../mutex/mutex.h"

typedef pthread_cond_t cond_t;

// region public functions prototypes

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
extern void wait_cond(cond_t * cond, mutex_t * mutex);

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
extern void init_cond(cond_t * cond, int proc_shared);

/**
 * @def destroy_cond
 * @brief Function that destroys a given condition variable.
 *
 * @param cond
 * Condition variable to be destroyed.
 *
 * @throws ConditionVariableDestructionException if is not possible to destroy the condition variable.
 *
 */
extern void destroy_cond(cond_t * cond);

/**
 * @def notify_cond_all
 * @brief Function that notifies all of the threads waiting for the given condition variable.
 *
 * @param cond
 * Condition variable whose state changes are to be broadcast.
 *
 * @throws ConditionVariableBroadcastException if it not possible to broadcast the condition variable state changes.
 *
 */
extern void notify_cond_all(cond_t * cond);

/**
 * @def notify_cond
 * @brief Function that notifies a thread that is waiting for the given condition variable.
 *
 * @param cond
 * Condition variable whose state changes are to be signaled.
 *
 * @throws ConditionVariableException if it is not possible to signal the condition variable state changes.
 *
 */
extern void notify_cond(cond_t * cond);

// endregion public functions prototypes

#endif //IPCS_SYNC_COND_VAR_H
