/*
 * Authors:
 *  - Joao Filipe Guiomar Artur, 2019217853
 *  - Sancho Amaral Simoes, 2019217590
 *
 * Date of creation: 02/04/2021
 */

#ifndef RACESIMULATOR_IPC_MANAGER_H
#define RACESIMULATOR_IPC_MANAGER_H

#include <stddef.h>
#include <semaphore.h>

/**
 * Create shared memory zone.
 * @param size Memory space needed for the shared memory.
 * @param shm_id_p Pointer to store the shared memory id.
 * @return Void pointer to the shared memory zone;
 */
void * create_shm(size_t size, int * shm_id_p);

/**
 * Destroy a shared memory zone.
 * @param shmid ID of the shared memory zone.
 * @param shared_mem Pointer to the shared memory zone.
 */
void destroy_shm(int shmid, void * shared_mem);

/**
 * Destroy a POSIX named semaphore.
 * @param sem_name Name of the POSIX named semaphore.
 * @param sem POSIX named semaphore.
 */
void destroy_sem(const char * sem_name, sem_t * sem);

/**
 * Destroy an array of POSIX named semaphores.
 * @param sem_array Array of POSIX named semaphore.
 * @param num Number of POSIX named semaphores contained in the array.
 * @param sem_name_prefix Name prefix of the POSIX named semaphores.
 */
void destroy_sem_array(sem_t ** sem_array, int num, const char * sem_name_prefix);

/**
 * Put on wait a given POSIX named semaphore.
 * Throws exception in case of error.
 * @param sem POSIX named semaphore.
 * @param sem_name Name of the semaphore.
 */
void wait_sem(sem_t * sem, const char * sem_name);

/**
 * Post a given POSIX named semaphore.
 * @param sem POSIX named semaphore.
 * @param sem_name Name of the POSIX named semaphore.
 */
void post_sem(sem_t * sem, const char * sem_name);

/**
 * Create a POSIX named semaphore.
 * @param sem_name Name of the semaphore.
 * @param value Initial value of the semaphore.
 * @return Pointer to the created semaphore.
 */
sem_t * create_sem(const char * sem_name, int value);

/**
 * Create an array of POSIX named semaphores.
 * @param num Number of semaphores contained in the array.
 * @param sem_name_prefix Name prefix of the semaphores.
 * @param value Initial value of the semaphore.
 * @return Pointer to the array of semaphores.
 */
sem_t ** create_sem_array(int num, const char * sem_name_prefix, int value);

#endif //RACESIMULATOR_IPC_MANAGER_H