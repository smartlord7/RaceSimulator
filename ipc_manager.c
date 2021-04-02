#include "structs/shared_memory.h"
#include "global.h"
#include "util/error_handler.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>

#include "util/debug.h"


void create_ipcs(void){
    if ((shm_id = shmget(IPC_PRIVATE, sizeof(shared_memory), IPC_CREAT | 0700)) < 0){
        throw_error(ERROR_SHM_CREATION);
    }

    DEBUG_MSG(SHM_CREATED, shm_id)

    if ((mem_struct = (shared_memory *) shmat(shm_id, NULL, 0)) == (void *) - 1){
        throw_error(ERROR_MEMORY_ATTACHMENT, shm_id);
    }

    DEBUG_MSG(SHM_ATTACHED, shm_id)

    sem_unlink(MUTEX);

    if ((mutex = sem_open(MUTEX, O_CREAT|O_EXCL, 0700, 1)) == SEM_FAILED){
        throw_error(ERROR_SEM_CREATION, MUTEX);
    }

    DEBUG_MSG(SEM_CREATED, MUTEX)
}

void destroy_ipcs(void){
    if(shmdt(mem_struct) == -1){
        throw_error(ERROR_MEMORY_DETACHMENT, shm_id);
    }

    DEBUG_MSG(SHM_DETACHED, shm_id)

    if (shmctl(shm_id, IPC_RMID, NULL) == -1){
        throw_error(ERROR_MEMORY_RM, shm_id);
    }

    DEBUG_MSG(SHM_REMOVED, shm_id)

    if (sem_close(mutex) == -1){
        throw_error(ERROR_SEM_CLOSE, MUTEX);
    }

    DEBUG_MSG(SEM_CLOSED, MUTEX)

    if (sem_unlink(MUTEX) == -1){
        throw_error(ERROR_UNLINK_SEM, MUTEX);
    }

    DEBUG_MSG(SEM_UNLINKED, MUTEX);
}