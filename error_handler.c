#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void throw_error(const char * error_msg, ...) {

    va_list args;
    va_start(args, error_msg);

    vfprintf(stderr, error_msg, args);

    va_end(args);

    exit(EXIT_FAILURE);
}
