#ifndef IPCS_SHM_H
#define IPCS_SHM_H

#define true 1
#define false 0
#define ALL_PERMS_RW 0666

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
extern void * create_shm(size_t size, int * shm_id_p);

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
extern void destroy_shm(int shm_id, void * shared_mem);

#endif //IPCS_SHM_H
