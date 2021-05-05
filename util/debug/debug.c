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
#include "../global.h"

// endregion dependencies

// region public functions

void debug_msg(const char * file_name, int line, const char * msg, ...) {
    va_list args;
    va_start(args, msg);

    char buffer[MEDIUM_SIZE], buffer2[LARGE_SIZE];

    snprintf(buffer, MEDIUM_SIZE, "\n--FILE %s, LINE %d--\n", file_name, line);
    vsnprintf(buffer2, LARGE_SIZE, msg, args);
    fprintf(stdout, "%s%s\n", buffer, buffer2);

    va_end(args);
}

// endregion public functions