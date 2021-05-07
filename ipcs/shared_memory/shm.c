#include "stdlib.h"
#include "assert.h"
#include <sys/shm.h>
#include "../../util/debug/debug.h"
#include "../../util/exception_handler/exception_handler.h"
#include "shm.h"

void * create_shm(size_t size, int * shm_id_p) {
    assert(size != 0 && shm_id_p != NULL);

    int created_shm_id;
    void * shm = NULL;

    if ((created_shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | ALL_PERMS_RW)) <= 0){
        throw_and_exit(SHM_CREATE_EXCEPTION, "");
    }

    DEBUG_MSG(SHM_CREATE, created_shm_id)

    if ((shm = shmat(created_shm_id, NULL, 0)) == (void *) - 1){
        throw_and_exit(SHM_ATTACH_EXCEPTION, created_shm_id);
    }

    DEBUG_MSG(SHM_ATTACH, created_shm_id)

    * shm_id_p = created_shm_id;

    return shm;
}

void destroy_shm(int shm_id, void * shared_mem) {
    assert(shm_id > 0 && shared_mem != NULL);

    if(shmdt(shared_mem) == -1){
        throw_and_exit(SHM_DETACH_EXCEPTION, shm_id);
    }

    DEBUG_MSG(SHM_DETACH, shm_id)

    if (shmctl(shm_id, IPC_RMID, NULL) == -1){
        throw_and_exit(SHM_REMOVE_EXCEPTION, shm_id);
    }

    DEBUG_MSG(SHM_REMOVED, shm_id)
}