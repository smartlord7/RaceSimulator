#include <stdio.h>
#include <stdarg.h>

void debug_msg(const char * file, int line, const char * msg, ...) {
    va_list args;
    va_start(args, msg);

    fprintf(stdout, "--FILE %s, LINE %d--\n", file, line);
    vfprintf(stdout, msg, args);
    fprintf(stdout, "\n\n");

    va_end(args);
}