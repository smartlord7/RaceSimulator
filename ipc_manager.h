/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 02/04/2021
*/

#ifndef RACESIMULATOR_C_IPC_MANAGER_H
#define RACESIMULATOR_C_IPC_MANAGER_H

// region dependencies

#include <stddef.h>
#include <semaphore.h>

// endregion dependencies

// region constants

#define USER_PERMS_ALL 0700
#define ALL_PERMS_RW 0666
#define true 1
#define false 0
#define DEBUG false
#define MAX_SEM_LABEL_SIZE 20

// endregion constants

// region public functions

/**
 * @def create_shm
 * @brief Function that creates a shared memory zone.
 *
 * @param size
 * Specifies the size, in bytes, of the requested shared memory.
 *
 * @param shm_id_p
 * Pointer to int to where the shared memory zone id should stored.
 *
 * @return a void pointer to the shared memory zone.
 *
 * @throws ShmCreationException if the shared memory zone can't be created.
 *         ShmAttachmentException if the the shared memory zone can't be attached to the current process address zone.
 *
 */
void * create_shm(size_t size, int * shm_id_p);

/**
 * @def destroy_shm
 * @brief Function that destroys a shared memory zone.
 *
 * @param shm_id
 * The id of the shared memory zone.
 *
 * @param shared_mem
 * Pointer to the shared memory zone.
 *
 * @throws ShmAttachmentException if the the shared memory zone can't be detached from the current process address zone.
 *         ShmRemovalException if the shared memory zone can't be removed.
 *
 */
void destroy_shm(int shm_id, void * shared_mem);

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
sem_t * create_sem(const char * sem_name, int initial_value);

/**
 * @def destroy_sem
 * @brief Function that closes and unlinks a POSIX name semaphores.
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
void destroy_sem(const char * sem_name, sem_t * sem);

/**
 * @def create_sem_array
 * @brief Function that creates an array of POSIX named semaphores.
 *
 * @param num
 * The number of POSIX named semaphores to be created.
 *
 * @param sem_name_prefix
 * The name prefix of the POSIX named semaphores.
 *
 * @param initial_value
 * The initial value of each POSIX named semaphore.
 *
 * @return a pointer to the array of pointers to POSIX named semaphores.
 *
 * @throws SemCreationException if a POSIX named semaphore can't be created.
 *
 */
sem_t ** create_sem_array(int num, const char * sem_name_prefix, int initial_value);

/**
 * @def destroy_sem_array
 * @brief Function that closes and unlinks an array of POSIX named semaphores.
 *
 * @param sem_array
 * The array of pointers to POSIX named semaphores.
 *
 * @param num
 * The number of pointers to POSIX named semaphores contained in the array.
 *
 * @param sem_name_prefix
 * The prefix of the POSIX named semaphores.
 *
 * @throws SemCloseException if a POSIX named semaphore can't be closed.
 *         SemUnlinkException if a POSIX named semaphore can't be unlinked.
 *
 */
void destroy_sem_array(sem_t ** sem_array, int num, const char * sem_name_prefix);

/**
 * @def wait_sem
 * @brief Waits on a given POSIX named semaphore.
 *
 * @param sem
 * A pointer to a POSIX named semaphore.
 *
 * @param sem_name
 * The name of the POSIX named semaphore.
 *
 * @throws SemWaitException if the impossibility to wait on a POSIX named semaphore.
 *
 */
void wait_sem(sem_t * sem, const char * sem_name);

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
 * @throws SemPostException if the impossibility to post a POSIX named semaphore.
 *
 */
void post_sem(sem_t * sem, const char * sem_name);

// endregion public functions

#endif //RACESIMULATOR_C_IPC_MANAGER_H