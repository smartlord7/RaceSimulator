#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void throw_error(int exit_process, const char * error_msg, ...) {

    va_list args;
    va_start(args, error_msg);

    fprintf(stderr, "\n");
    vfprintf(stderr, error_msg, args);
    fprintf(stderr, "\n\n");

    va_end(args);

    if (exit_process) {
        exit(EXIT_FAILURE);
    }
}

