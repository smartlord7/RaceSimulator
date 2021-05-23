/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef IPCS_MUTEX_H
#define IPCS_MUTEX_H

// region constants

#define true 1
#define false 0

// endregion constants

// region forward declarations

typedef pthread_mutex_t mutex_t;

// endregion forward declarations

// region public functions prototypes

/**
 * @def init_mutex
 * @brief Function that initializes a given pthread mutex.
 *
 * @param mutex
 * pthread mutex to be initialized.
 *
 * @param proc_shared
 * Flag that indicates if the mutex is to be shared between processes.
 *
 * @throws MutexShareException if it is not possible to share the mutex between processes.
 *         MutexInitializeException if it is not possible to initialize the mutex.
 */
extern void init_mutex(mutex_t * mutex, int proc_shared);

/**
 * @def destroy_mutex
 * @brief Function that destroys a given pthread mutex.
 *
 * @param mutex
 * pthread mutex to be destroyed.
 *
 * @throws MutexCloseException if it is not possble to destroy the mutex.
 */
extern void destroy_mutex(mutex_t * mutex);

/**
 * @def lock_mutex
 * @brief Function that locks a given pthread mutex.
 *
 * @param mutex
 * pthread mutex to be locked.
 *
 * @throws MutexLockException if an error occurred whilst locking the mutex.
 */
extern void lock_mutex(mutex_t * mutex);

/**
 * @def unlock_mutex
 * @brief Function that unlocks a given pthread mutex.
 *
 * @param mutex
 * pthread mutex to be unlocked.
 *
 * @throws MutexUnlockException if an error occurred whilst unlocking the mutex.
 */
extern void unlock_mutex(mutex_t * mutex);

// endregion public functions prototypes

#endif //IPCS_MUTEX_H
