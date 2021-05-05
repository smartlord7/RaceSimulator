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
#include <sys/msg.h>

// endregion dependencies

// region constants

#define USER_PERMS_ALL 0700
#define ALL_PERMS_RW 0666
#define true 1
#define false 0
#define DEBUG true
#define MAX_SEM_LABEL_SIZE 20
#define SEM_ARRAY "SEM_ARRAY"
#define MESSAGE_SIZE sizeof(message_t) - sizeof(long)
#define UNNAMED_PIPE "UNNAMED PIPE"
#define NAMED PIPE "NAMED PIPE"

// endregion constants

typedef struct message_t message_t;
struct message_t {
    long priority;
    int type_of_malfunction;
};

// region public functions

/**
 * @def create_shm
 * @brief Function that creates a shared memory zone.
 *
 * @param size
 * Specifies the size, in bytes, of the requested shared memory.
 *
 * @param shm_id_p
 * Pointer to int to where the shared memory zone id should be stored.
 *
 * @return a void pointer to the shared memory zone.
 *
 * @throws ShmCreationException if the requested shared memory zone can't be created.
 * @throws ShmAttachmentException if the the shared memory zone can't be attached to the current process address zone.
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
 * A ointer to the shared memory zone.
 *
 * @throws ShmAttachmentException if the the shared memory zone can't be detached from the current process address zone.
 * @throws ShmRemovalException if the shared memory zone can't be removed.
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
void destroy_sem(const char * sem_name, sem_t * sem);

/**
 * @def create_sem_array
 * @brief Function that creates an array of POSIX named semaphores.
 *
 * @param num
 * The number of POSIX named semaphores to be created.
 *
 * @param sem_name_prefix
 * The name prefix of each POSIX named semaphore of the array.
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
 * A pointer to an array of pointers to POSIX named semaphores.
 *
 * @param num
 * The number of pointers to POSIX named semaphores contained in the array.
 *
 * @param sem_name_prefix
 * The prefix of each POSIX named semaphore in the array.
 *
 * @throws SemCloseException if a POSIX named semaphore can't be closed.
 * @throws SemUnlinkException if a POSIX named semaphore can't be unlinked.
 *
 */
void destroy_sem_array(sem_t ** sem_array, int num, const char * sem_name_prefix);

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
 * @throws SemPostException given the impossibility of posting the POSIX named semaphore.
 *
 */
void post_sem(sem_t * sem, const char * sem_name);

int create_msg_queue();

int destroy_message_queue(int msgq_id);

int receive_message(int msgq_id, message_t * message, int priority);

int send_message(int msgq_id, message_t * message);

int create_unnamed_pipe(int file_descriptors[]);

int destroy_file_descriptor(char * context, int file_descriptors[], int num_of_file_descriptors);

int create_named_pipe(const char * pipe_name);

int open_named_pipe(char * pipe_name, int * file_descriptor, int mode);

// endregion public functions

#endif //RACESIMULATOR_C_IPC_MANAGER_H