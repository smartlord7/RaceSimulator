/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 02/04/2021
*/

#ifndef IPCS_SEM_H
#define IPCS_SEM_H

// region constants

#define true 1
#define false 0
#define MAX_SEM_LABEL_SIZE 64
#define SEM_ARRAY "SEM_ARRAY"
#define USER_PERMS_ALL 0700

// endregion constants

// region public functions prototypes

/**
 * @def create_sem
 * @brief Function that creates a POSIX named semaphore,
 *
 * @param sem_name
 * The name of the POSIX named semaphore.
 *
 * @param initial_value
 * The initial initial_value of the POSIX named semaphore.
 *
 * @return A pointer to the created POSIX named semaphore.
 *
 * @throws SemCreationException if the POSIX named semaphore can't be created.
 *
 */
extern sem_t * create_sem(const char * sem_name, int initial_value);

/**
 * @def destroy_sem
 * @brief Function that closes and unlinks a POSIX name semaphore.
 *
 * @param sem_name
 * The name of the POSIX named semaphore.
 *
 * @param sem
 * A pointer to the POSIX named semaphore.
 *
 * @throws SemCloseException if the POSIX named semaphore can't be closed.
 *         SemUnlinkException if the POSIX named semaphore can't be unlinked.
 *
 */
extern void destroy_sem(const char * sem_name, sem_t * sem);

/**
 * @def create_sem_array
 * @brief Function that creates an array of POSIX named semaphore.
 *
 * @param num
 * The number of POSIX named semaphore to be created.
 *
 * @param sem_name_prefix
 * The name prefix of each POSIX named semaphore of the array.
 *
 * @param initial_value
 * The initial value of each POSIX named semaphore.
 *
 * @return a pointer to the array of pointers to POSIX named semaphore.
 *
 * @throws SemCreationException if a POSIX named semaphore can't be created.
 *
 */
extern sem_t ** create_sem_array(int num, const char * sem_name_prefix, int initial_value);

/**
 * @def destroy_sem_array
 * @brief Function that closes and unlinks an array of POSIX named semaphore.
 *
 * @param sem_array
 * A pointer to an array of pointers to POSIX named semaphore.
 *
 * @param num
 * The number of pointers to POSIX named semaphore contained in the array.
 *
 * @param sem_name_prefix
 * The prefix of each POSIX named semaphore in the array.
 *
 * @throws SemCloseException if a POSIX named semaphore can't be closed.
 * @throws SemUnlinkException if a POSIX named semaphore can't be unlinked.
 *
 */
extern void destroy_sem_array(sem_t ** sem_array, int num, const char * sem_name_prefix);

/**
 * @def wait_sem
 * @brief Function that waits for a given POSIX named semaphore.
 *
 * @param sem
 * A pointer to a POSIX named semaphore.
 *
 * @param sem_name
 * The name of the POSIX named semaphore.
 *
 * @throws SemWaitException given the impossibility of waiting for the POSIX named semaphore.
 *
 */
extern void wait_sem(sem_t * sem, const char * sem_name);

/**
 * @def post_sem
 * @brief Posts a given POSIX named semaphore.
 *
 * @param sem
 * A pointer to a POSIX named semaphore.
 *
 * @param sem_name
 * The name of the POSIX named semaphore.
 *
 * @throws SemPostException given the impossibility of posting the POSIX named semaphore.
 *
 */
extern void post_sem(sem_t * sem, const char * sem_name);

// endregion public functions prototypes

#endif //IPCS_SEM_H
