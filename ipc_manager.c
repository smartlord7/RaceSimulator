#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include "global.h"
#include "util/error_handler.h"
#include "util/debug.h"

sem_t ** create_sem_array(int num, const char * sem_name_prefix) {
    sem_t ** sem_array;

    if ((sem_array = (sem_t **) malloc(num * sizeof(sem_t *))) == NULL) {
        throw_error_end_exit(ERROR_MEMORY_ALLOCATION, "sem array");
    }

    int i = 0;

    while (i < num) {
        char sem_name[MAX_LABEL_SIZE];
        snprintf(sem_name, sizeof(sem_name), "%s%d", sem_name_prefix, i);

        sem_unlink(sem_name);

        if ((sem_array[i] = sem_open(sem_name, O_CREAT | O_EXCL, 0700, 1)) == SEM_FAILED){
            throw_error_end_exit(ERROR_SEM_CREATION, sem_name);
        }

        i++;
    }

    return sem_array;
}

void destroy_sem_array(sem_t ** sem_array, int num, const char * sem_name_prefix) {
    int i = 0;

    while (i < num) {
        char sem_name[MAX_LABEL_SIZE];
        snprintf(sem_name, sizeof(sem_name), "%s%d", sem_name_prefix, i);

        if (sem_close(sem_array[i]) == -1){
            throw_error_end_exit(ERROR_SEM_CLOSE, sem_name);
        }

        sem_unlink(sem_name);

        i++;
    }
}

void create_ipcs(int num_teams){
    if ((shm_id = shmget(IPC_PRIVATE, sizeof(shared_memory), IPC_CREAT | 0666)) <= 0){
        throw_error_end_exit(ERROR_SHM_CREATION, NULL);
    }

    DEBUG_MSG(SHM_CREATED, shm_id)

    if ((mem_struct = (shared_memory *) shmat(shm_id, NULL, 0)) == (void *) - 1){
        throw_error_end_exit(ERROR_MEMORY_ATTACHMENT, shm_id);
    }

    DEBUG_MSG(SHM_ATTACHED, shm_id)

    sem_unlink(MUTEX);

    if ((mutex = sem_open(MUTEX, O_CREAT | O_EXCL, 0700, 1)) == SEM_FAILED){
        throw_error_end_exit(ERROR_SEM_CREATION, MUTEX);
    }

    DEBUG_MSG(SEM_CREATED, MUTEX)

    sem_unlink(RACE_START);

    if ((race_start = sem_open(RACE_START, O_CREAT | O_EXCL, 0700, 0)) == SEM_FAILED){
        throw_error_end_exit(ERROR_SEM_CREATION, RACE_START);
    }

    DEBUG_MSG(SEM_CREATED, RACE_START)

    boxes_availability = create_sem_array(num_teams, BOX_SEM_PREFIX);

    DEBUG_MSG(SEM_CREATED, BOX_SEM_PREFIX)
}

void destroy_ipcs(int num_teams){
    if(shmdt(mem_struct) == -1){
        throw_error_end_exit(ERROR_MEMORY_DETACHMENT, shm_id);
    }

    DEBUG_MSG(SHM_DETACHED, shm_id)

    if (shmctl(shm_id, IPC_RMID, NULL) == -1){
        throw_error_end_exit(ERROR_MEMORY_RM, shm_id);
    }

    DEBUG_MSG(SHM_REMOVED, shm_id)

    if (sem_close(mutex) == -1){
        throw_error_end_exit(ERROR_SEM_CLOSE, MUTEX);
    }

    DEBUG_MSG(SEM_CLOSED, MUTEX)

    if (sem_unlink(MUTEX) == -1){
        throw_error_end_exit(ERROR_UNLINK_SEM, MUTEX);
    }

    DEBUG_MSG(SEM_UNLINKED, MUTEX);

    if (sem_close(race_start) == -1){
        throw_error_end_exit(ERROR_SEM_CLOSE, RACE_START);
    }

    DEBUG_MSG(SEM_CLOSED, RACE_START)

    if (sem_unlink(RACE_START) == -1){
        throw_error_end_exit(ERROR_UNLINK_SEM, RACE_START);
    }

    DEBUG_MSG(SEM_UNLINKED, RACE_START);

    destroy_sem_array(boxes_availability, num_teams, BOX_SEM_PREFIX);
}