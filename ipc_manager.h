#ifndef RACESIMULATOR_IPC_MANAGER_H
#define RACESIMULATOR_IPC_MANAGER_H

#include <stddef.h>
#include <semaphore.h>

/**
 * Create shared memory zone.
 * @param size Memory space needed for the shared memory.
 * @param shm_id_p
 * @return
 */
void * create_shm(size_t size, int * shm_id_p);

/**
 *
 * @param shmid
 * @param shared_mem
 */
void destroy_shm(int shmid, void * shared_mem);

/**
 *
 * @param sem_name
 * @param sem
 */
void destroy_sem(const char * sem_name, sem_t * sem);

/**
 *
 * @param sem_array
 * @param num
 * @param sem_name_prefix
 */
void destroy_sem_array(sem_t ** sem_array, int num, const char * sem_name_prefix);

/**
 *
 * @param num_teams
 */
void create_ipcs(int num_teams);

/**
 *
 * @param num_teams
 */
void destroy_ipcs(int num_teams);

/**
 *
 * @param sem
 * @param sem_name
 */
void wait_sem(sem_t * sem, const char * sem_name);

/**
 *
 * @param sem
 * @param sem_name
 */
void post_sem(sem_t * sem, const char * sem_name);

/**
 *
 * @param sem_name
 * @return
 */
sem_t * create_sem(const char * sem_name);

/**
 *
 * @param num
 * @param sem_name_prefix
 * @return
 */
sem_t ** create_sem_array(int num, const char * sem_name_prefix);

#endif //RACESIMULATOR_IPC_MANAGER_H