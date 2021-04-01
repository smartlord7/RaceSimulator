#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define true 1
#define false 0

static int has_error = false;

void throw_error(int exit_program, const char * error_msg, ...) {

    va_list args;
    va_start(args, error_msg);

    vfprintf(stderr, error_msg, args);
    fprintf(stderr, "\n");

    va_end(args);

    has_error = true;

    if (exit_program) {
        exit(EXIT_FAILURE);
    }
}

void reset(void) {
    has_error = false;
}

void exit_later(void) {
    if (has_error) {
        exit(EXIT_FAILURE);
    }
}

