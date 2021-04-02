#include <unistd.h>
#include <sys/wait.h>
#include "error_handler.h"

#define true 1
#define false 0

void create_process(const char * proc_name, void (* worker)(void *), void * params) {
    pid_t child_proc_id;

    if ((child_proc_id = fork()) == 0) {
        worker(params);
    } else if (child_proc_id < 0) {
        throw_error_end_exit(ERROR_CREATE_PROCESS, proc_name);
    }
}

void wait_all() {
    while (wait(NULL) != -1);
}


