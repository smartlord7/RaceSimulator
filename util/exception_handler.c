#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <semaphore.h>
#include <assert.h>

sem_t * exc_mutex = NULL;
void (* clean_func)(void *) = NULL;
void * clean_func_params = NULL;

void exc_handler_init(sem_t * sem, void (* clean_function)(void *), void * clean_function_params) {
    assert(clean_function != NULL);

    exc_mutex = sem;
    clean_func = clean_function;
    clean_func_params = clean_function_params;
}

void throw_exception(const char * file_name, int line, int exit_process, const char * exception_msg, ...) {
    assert(exception_msg != NULL && file_name != NULL && line > 0);

    va_list args;
    va_start(args, exception_msg);

    if (exc_mutex != NULL) {
        assert(sem_wait(exc_mutex) != -1);
    }

    fprintf(stderr, "\nEXCEPTION AT LINE %d, FILE %s\n", line, file_name);
    vfprintf(stderr, exception_msg, args);
    fprintf(stderr, "\n");

    if (exc_mutex != NULL) {
        assert(sem_post(exc_mutex) != -1);
    }

    va_end(args);

    if (exit_process) {
        clean_func(clean_func_params);
        exit(EXIT_FAILURE);
    }
}