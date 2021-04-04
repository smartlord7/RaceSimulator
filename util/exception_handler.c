#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <semaphore.h>
#include <assert.h>

void sync_throw_exception(sem_t * sem, const char * file_name, int line, int exit_process, const char * exception_msg, ...) {
    assert(sem != NULL && exception_msg != NULL && file_name != NULL && line > 0);

    va_list args;
    va_start(args, exception_msg);

    assert(sem_wait(sem) != -1);

    fprintf(stderr, "\nEXCEPTION AT LINE %d, FILE %s\n", line, file_name);
    vfprintf(stderr, exception_msg, args);
    fprintf(stderr, "\n");

    assert(sem_post(sem) != -1);

    va_end(args);

    if (exit_process) {
        exit(EXIT_FAILURE);
    }
}

void throw_exception(const char * file_name, int line, int exit_process, const char * exception_msg, ...) {
    assert(exception_msg != NULL && file_name != NULL && line > 0);

    va_list args;
    va_start(args, exception_msg);

    fprintf(stderr, "\nEXCEPTION AT LINE %d, FILE %s\n", line, file_name);
    vfprintf(stderr, exception_msg, args);
    fprintf(stderr, "\n");

    va_end(args);

    if (exit_process) {
        exit(EXIT_FAILURE);
    }
}



