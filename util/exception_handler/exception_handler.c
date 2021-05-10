/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "assert.h"
#include "errno.h"
#include "../global.h"

// endregion dependencies

// region global variables

void (* clean_func)(void *) = NULL;
void * clean_func_params = NULL;
int force_exit = false;

// endregion global variables

// region public functions

void exc_handler_init(void (* clean_function)(void *), void * clean_function_params) {
    assert(clean_function != NULL);

    clean_func = clean_function;
    clean_func_params = clean_function_params;
}

void throw_exception(const char * file_name, int line, int exit_process, const char * exception_msg, ...) {
    assert(exception_msg != NULL && file_name != NULL && line > 0);

    va_list args;
    va_start(args, exception_msg);

    char buffer[MEDIUM_SIZE], buffer2[LARGE_SIZE];

    snprintf(buffer, MEDIUM_SIZE, "\nEXCEPTION AT LINE %d, FILE %s\n", line, file_name);
    vsnprintf(buffer2, LARGE_SIZE, exception_msg, args);
    fprintf(stderr, "%s%s\n[ERRNO:%d] %s", buffer, buffer2, errno, strerror(errno));

    va_end(args);

    if (exit_process) {
        if (clean_func && !force_exit) {
            clean_func(clean_func_params);
        }
        exit(EXIT_FAILURE);
    }
}

// endregion public functions