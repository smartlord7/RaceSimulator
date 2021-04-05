/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 02/04/2021
*/

// region dependencies

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stddef.h>
#include "exception_handler.h"
#include "ipc_manager.h"
#include "debug.h"

// endregion dependencies

// region public functions

void wait_sem(sem_t * sem, const char * sem_name) {
    assert(sem != NULL && sem_name != NULL);

    if (sem_wait(sem) == -1) {
        throw_exception_and_exit(SEM_WAIT_EXCEPTION, sem_name);
    }
}

void post_sem(sem_t * sem, const char * sem_name) {
    assert(sem != NULL && sem_name != NULL);

    if (sem_post(sem) == -1) {
        throw_exception_and_exit(SEM_POST_EXCEPTION, sem_name);
    }
}

void * create_shm(size_t size, int * shm_id_p) {
    assert(size != 0 && shm_id_p != NULL);

    int created_shm_id;
    void * shm = NULL;

    if ((created_shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | ALL_PERMS_RW)) <= 0){
        throw_exception_and_exit(SHM_CREATION_EXCEPTION, NULL);
    }

    DEBUG_MSG(SHM_CREATED, created_shm_id)

    if ((shm = shmat(created_shm_id, NULL, 0)) == (void *) - 1){
        throw_exception_and_exit(SHM_ATTACHMENT_EXCEPTION, created_shm_id);
    }

    DEBUG_MSG(SHM_ATTACHED, created_shm_id)

    * shm_id_p = created_shm_id;

    return shm;
}

void destroy_shm(int shm_id, void * shared_mem) {
    assert(shared_mem != NULL);

    if(shmdt(shared_mem) == -1){
        throw_exception_and_exit(SHM_DETACHMENT_EXCEPTION, shm_id);
    }

    DEBUG_MSG(SHM_DETACHED, shm_id)

    if (shmctl(shm_id, IPC_RMID, NULL) == -1){
        throw_exception_and_exit(SHM_REMOVAL_EXCEPTION, shm_id);
    }

    DEBUG_MSG(SHM_REMOVED, shm_id)
}

sem_t * create_sem(const char * sem_name, int initial_value) {
    assert(sem_name != NULL && initial_value >= 0);

    sem_t * sem;

    sem_unlink(sem_name);

    if ((sem = sem_open(sem_name, O_CREAT | O_EXCL, USER_PERMS_ALL, initial_value)) == SEM_FAILED){
        throw_exception_and_exit(SEM_CREATION_EXCEPTION, sem_name);
    }

    return sem;
}

void destroy_sem(const char * sem_name, sem_t * sem) {
    assert(sem_name != NULL && sem != NULL);

    if (sem_close(sem) == -1){
        throw_exception_and_exit(SEM_CLOSE_EXCEPTION, sem_name);
    }

    DEBUG_MSG(SEM_CLOSED, sem_name)

    if (sem_unlink(sem_name) == -1){
        throw_exception_and_exit(SEM_UNLINK_EXCEPTION, sem_name);
    }

    DEBUG_MSG(SEM_UNLINKED, sem_name);
}

sem_t ** create_sem_array(int num, const char * sem_name_prefix, int initial_value) {
    assert(num != 0 && sem_name_prefix != NULL && initial_value > 0);

    sem_t ** sem_array;

    if ((sem_array = (sem_t **) malloc(num * sizeof(sem_t *))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, "sem array");
    }

    int i = 0;

    while (i < num) {
        char sem_name[MAX_SEM_LABEL_SIZE];
        snprintf(sem_name, sizeof(sem_name), "%s%d", sem_name_prefix, i);

        sem_array[i] = create_sem(sem_name, initial_value);

        DEBUG_MSG(SEM_CREATED, sem_name)

        i++;
    }

    return sem_array;
}

void destroy_sem_array(sem_t ** sem_array, int num, const char * sem_name_prefix) {
    assert(sem_array != NULL && num != 0 && sem_name_prefix != NULL);

    int i = 0;

    while (i < num) {
        char sem_name[MAX_SEM_LABEL_SIZE];
        snprintf(sem_name, sizeof(sem_name), "%s%d", sem_name_prefix, i);

        destroy_sem(sem_name, sem_array[i]);

        i++;
    }

    free(sem_array);
}

// endregion public functions