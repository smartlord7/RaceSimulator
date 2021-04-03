#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stddef.h>
#include "global.h"
#include "util/error_handler.h"
#include "util/debug.h"

#define USER_PERMS_ALL 0700
#define ALL_PERMS_RW 0666

void * create_shm(size_t size, int * shm_id_p) {
    int created_shm_id;
    void * shm = NULL;

    if ((created_shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | ALL_PERMS_RW)) <= 0){
        throw_error_end_exit(ERROR_SHM_CREATION, NULL);
    }

    DEBUG_MSG(SHM_CREATED, created_shm_id)

    if ((shm = shmat(created_shm_id, NULL, 0)) == (void *) - 1){
        throw_error_end_exit(ERROR_MEMORY_ATTACHMENT, created_shm_id);
    }

    DEBUG_MSG(SHM_ATTACHED, shm_id)

    * shm_id_p = created_shm_id;

    return shm;
}

void destroy_shm(int shmid, void * shared_mem) {
    if(shmdt(shared_mem) == -1){
        throw_error_end_exit(ERROR_MEMORY_DETACHMENT, shmid);
    }

    DEBUG_MSG(SHM_DETACHED, shmid)

    if (shmctl(shmid, IPC_RMID, NULL) == -1){
        throw_error_end_exit(ERROR_MEMORY_RM, shmid);
    }

    DEBUG_MSG(SHM_REMOVED, shmid)
}

sem_t * create_sem(const char * sem_name) {
    sem_t * sem;

    sem_unlink(sem_name);

    if ((sem = sem_open(sem_name, O_CREAT | O_EXCL, USER_PERMS_ALL, 1)) == SEM_FAILED){
        throw_error_end_exit(ERROR_SEM_CREATION, sem_name);
    }

    return sem;
}

void destroy_sem(const char * sem_name, sem_t * sem) {
    if (sem_close(sem) == -1){
        throw_error_end_exit(ERROR_SEM_CLOSE, sem_name);
    }

    DEBUG_MSG(SEM_CLOSED, sem_name)

    if (sem_unlink(sem_name) == -1){
        throw_error_end_exit(ERROR_UNLINK_SEM, sem_name);
    }

    DEBUG_MSG(SEM_UNLINKED, sem_name);
}

sem_t ** create_sem_array(int num, const char * sem_name_prefix) {
    sem_t ** sem_array;

    if ((sem_array = (sem_t **) malloc(num * sizeof(sem_t *))) == NULL) {
        throw_error_end_exit(ERROR_MEMORY_ALLOCATION, "sem array");
    }

    int i = 0;

    while (i < num) {
        char sem_name[MAX_LABEL_SIZE];
        snprintf(sem_name, sizeof(sem_name), "%s%d", sem_name_prefix, i);

        sem_array[i] = create_sem(sem_name);

        DEBUG_MSG(SEM_CREATED, sem_name)

        i++;
    }

    return sem_array;
}

void destroy_sem_array(sem_t ** sem_array, int num, const char * sem_name_prefix) {
    int i = 0;

    while (i < num) {
        char sem_name[MAX_LABEL_SIZE];
        snprintf(sem_name, sizeof(sem_name), "%s%d", sem_name_prefix, i);

        destroy_sem(sem_name, sem_array[i]);

        i++;
    }
}

void create_ipcs(int num_teams){
    mem_struct = create_shm(sizeof(shared_memory), &shm_id);

    mutex = create_sem(MUTEX);

    DEBUG_MSG(SEM_CREATED, MUTEX)

    race_start = create_sem(RACE_START);

    DEBUG_MSG(SEM_CREATED, RACE_START)

    boxes_availability = create_sem_array(num_teams, BOX_SEM_PREFIX);

    DEBUG_MSG(SEM_CREATED, BOX_SEM_PREFIX)
}

void destroy_ipcs(int num_teams){

    destroy_shm(shm_id, mem_struct);

    destroy_sem(MUTEX, mutex);

    destroy_sem(RACE_START, race_start);

    destroy_sem_array(boxes_availability, num_teams, BOX_SEM_PREFIX);
}