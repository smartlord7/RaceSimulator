/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include "../debug/exception_handler.h"
#include "process_manager.h"

// endregion dependencies

// region public functions

void create_process(const char * proc_name, void (* worker)(void *), void * params) {
    assert(proc_name != NULL && worker != NULL);

    pid_t child_proc_id;

    if ((child_proc_id = fork()) == 0) {
        worker(params);
    } else if (child_proc_id < 0) {
        throw_and_exit(PROCESS_CREATE_EXCEPTION, proc_name);
    }
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

void create_thread(const char * thread_name, pthread_t * thread_p, void * (* worker)(void *), void * params) {
    assert(thread_name != NULL && thread_p != NULL && worker != NULL);

    if (pthread_create(thread_p, NULL, worker, (void *) params) != 0) {
        throw_and_exit(THREAD_CREATE_EXCEPTION, thread_name);
    }
}

void wait_threads(int num_threads, pthread_t * threads) {
    assert(num_threads > 0 && threads != NULL);

    int i = 0;

    while (i < num_threads) {
        if (pthread_join(threads[i], NULL) != 0) {
            throw_and_exit(THREAD_JOIN_EXCEPTION, "");
        }

        pthread_kill(threads[i], SIGKILL);

        i++;
    }
}

// endregion public functions