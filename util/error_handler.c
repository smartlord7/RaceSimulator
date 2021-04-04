#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <semaphore.h>
#include <assert.h>

void sync_throw_error(sem_t * sem, int exit_process, const char * error_msg, ...) {
    assert(sem != NULL && error_msg != NULL);

    va_list args;
    va_start(args, error_msg);

    assert(sem_wait(sem) != -1);

    fprintf(stderr, "\n");
    vfprintf(stderr, error_msg, args);
    fprintf(stderr, "\n");

    assert(sem_post(sem) != -1);

    va_end(args);

    if (exit_process) {
        exit(EXIT_FAILURE);
    }
}

void throw_error(int exit_process, const char * error_msg, ...) {
    assert(error_msg != NULL);

    va_list args;
    va_start(args, error_msg);

    fprintf(stderr, "\n");
    vfprintf(stderr, error_msg, args);
    fprintf(stderr, "\n");

    va_end(args);

    if (exit_process) {
        exit(EXIT_FAILURE);
    }
}



