/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
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

// region global variables

debug_level dbg_level;
int show_trace;

// endregion global variables

// region public functions

void debug_init(debug_level deb_level, int sh_origin) {
    dbg_level = deb_level;
    show_trace = sh_origin;
}

void debug_msg(const char * file_name, int line, const char * msg, debug_level deb_level, ...) {
    if (dbg_level != deb_level) {
        return;
    }

    va_list args;
    va_start(args, deb_level);

    char buffer[MEDIUM_SIZE], buffer2[LARGE_SIZE];
    if (show_trace) {
        snprintf(buffer, MEDIUM_SIZE, "\n--FILE %s, LINE %d--\n", file_name, line);
    }
    vsnprintf(buffer2, LARGE_SIZE, msg, args);

    if (show_trace) {
        fprintf(stdout, "%s%s\n", buffer, buffer2);
    } else {
        fprintf(stdout, "%s\n", buffer2);
    }

    va_end(args);
}

// endregion public functions