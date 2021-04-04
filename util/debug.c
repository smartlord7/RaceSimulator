#include <stdio.h>
#include <stdarg.h>
#include <semaphore.h>
#include <assert.h>

sem_t * deb_mutex;

void debug_init(sem_t * sem) {
    deb_mutex = sem;
}

void debug_msg(const char * file_name, int line, const char * msg, ...) {
    va_list args;
    va_start(args, msg);

    if (deb_mutex != NULL) {
        assert(sem_wait(deb_mutex) != -1);
    }

    fprintf(stdout, "\n--FILE %s, LINE %d--\n", file_name, line);
    vfprintf(stdout, msg, args);
    fprintf(stdout, "\n");

    if (deb_mutex != NULL) {
        assert(sem_post(deb_mutex) != -1);
    }

    va_end(args);
}