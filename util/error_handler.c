#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <semaphore.h>

void sync_throw_error(sem_t * sem, int exit_process, const char * error_msg, ...) {
    va_list args;
    va_start(args, error_msg);

    sem_wait(sem);

    fprintf(stderr, "\n");
    vfprintf(stderr, error_msg, args);
    fprintf(stderr, "\n");

    sem_post(sem);

    va_end(args);

    if (exit_process) {
        exit(EXIT_FAILURE);
    }
}

void throw_error(int exit_process, const char * error_msg, ...) {
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



