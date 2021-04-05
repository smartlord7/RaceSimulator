/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include <stdio.h>
#include <stdarg.h>
#include <semaphore.h>
#include <assert.h>

// endregion dependencies

// region global variables

sem_t * deb_mutex = NULL;

// endregion global variables

// region public functions

void debug_init(sem_t * mutex) {
    deb_mutex = mutex;
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

// endregion public functions