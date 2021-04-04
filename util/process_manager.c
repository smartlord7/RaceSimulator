#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include "exception_handler.h"

#define true 1
#define false 0

void create_process(const char * proc_name, void (* worker)(void *), void * params) {
    assert(proc_name != NULL && worker != NULL);

    pid_t child_proc_id;

    if ((child_proc_id = fork()) == 0) {
        worker(params);
    } else if (child_proc_id < 0) {
        throw_exception_and_exit(CREATION_PROCESS_EXCEPTION, proc_name);
    }
}

void wait_all() {
    while (wait(NULL) != -1);
}

void create_thread(const char * thread_name, pthread_t * thread_p, void * (* worker)(void *), void * params) {
    assert(thread_name != NULL && thread_p != NULL && worker != NULL);

    if (pthread_create(thread_p, NULL, worker, (void *) params) != 0) {
        throw_exception_and_exit(CREATION_THREAD_EXCEPTION, thread_name);
    }
}

void kill_all_threads(int num_threads, pthread_t * threads) {
    assert(num_threads > 0 && threads != NULL);

    int i = 0;

    while (i < num_threads) {
        pthread_join(threads[i], NULL);
        pthread_kill(threads[i], SIGKILL);

        i++;
    }
}