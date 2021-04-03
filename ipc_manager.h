#ifndef RACESIMULATOR_IPC_MANAGER_H
#define RACESIMULATOR_IPC_MANAGER_H

#include <stddef.h>
#include <semaphore.h>

void * create_shm(size_t size, int * shm_id_p);
void destroy_shm(int shmid, void * shared_mem);
sem_t * create_sem(const char * sem_name);
void destroy_sem(const char * sem_name, sem_t * sem);
sem_t ** create_sem_array(int num, const char * sem_name_prefix);
void destroy_sem_array(sem_t ** sem_array, int num, const char * sem_name_prefix);
void create_ipcs(int num_teams);
void destroy_ipcs(int num_teams);

#endif //RACESIMULATOR_IPC_MANAGER_H