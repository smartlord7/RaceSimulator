/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include "../exception_handler/exception_handler.h"
#include "process_manager.h"
#include "../debug/debug.h"

// endregion dependencies

// region public functions

void create_process(const char * proc_name, void (* worker)(void *), void * params) {
    assert(proc_name != NULL && worker != NULL);

    pid_t child_proc_id;

    if ((child_proc_id = fork()) == 0) {
        worker(params);
        exit(EXIT_SUCCESS);
    }

    throw_if_exit(child_proc_id < 0, PROCESS_CREATE_EXCEPTION, proc_name);
}

void wait_procs() {
    while (wait(NULL) != -1);
}

void terminate_proc_grp(pid_t proc_group_id) {
    assert(proc_group_id > 0);

    char cmd[MAX_CMD_SIZE];

    snprintf(cmd, MAX_CMD_SIZE, "pkill -9 -g %d", proc_group_id);

    system(cmd);
}

void create_thread(const char * thread_name, pthread_t * thread, void * (* worker)(void *), void * params) {
    assert(thread_name != NULL && thread != NULL && worker != NULL);

    throw_if_exit(pthread_create(thread, NULL, worker, params) != 0, THREAD_CREATE_EXCEPTION, thread_name);
}

void wait_threads(int num_threads, pthread_t * threads) {
    assert(num_threads > 0 && threads != NULL);

    int i = 0;

    while (i < num_threads) {
        throw_if_stay(pthread_join(threads[i], NULL) != 0, THREAD_JOIN_EXCEPTION, "");
        i++;
    }
}

void exit_thread() {
    pthread_exit(NULL);
}

// endregion public functions