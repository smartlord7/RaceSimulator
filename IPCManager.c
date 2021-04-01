//
// Created by jfgar on 01/04/2021.
//
#include "global_header.h"
#include "error_handler.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>

void init(){
    if((shmid = shmget(IPC_PRIVATE, sizeof(mem_struct), IPC_CREAT|0700)) < 0){
        return throw_error(ERROR_SHM_CREATION);
    }

    if((mem_struct = (shared_memory*) shmat(shmid, NULL, 0)) == (shared_memory*) - 1){
        return throw_error(ERROR_MEMORY_ATTACHMENT, mem_struct);
    }

    if(sem_unlink(MUTEX) == -1){
        return throw_error(ERROR_UNLINK_SEM, MUTEX);
    }
    if((mutex = sem_open(MUTEX, O_CREAT|O_EXCL, 0700, 1)) == SEM_FAILED){
        return throw_error(ERROR_SEM_CREATION, MUTEX);
    }
}

void terminate(){
    if(shmdt(mem_struct) == -1){
        return throw_error(ERROR_MEMORY_DETACHMENT, mem_struct);
    }

    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        return throw_error(ERROR_MEMORY_RM, shmid);
    }

    if(sem_close(mutex) == -1){
        return throw_error(ERROR_SEM_CLOSE, MUTEX);
    }

    if(sem_unlink(MUTEX) == -1){
        return throw_error(ERROR_UNLINK_SEM, MUTEX);
    }
}