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

// region global variables

int debug_level, show_origin;

// endregion global variables

// region public functions

void debug_init(int deb_level, int sh_origin) {
    debug_level = deb_level;
    show_origin = sh_origin;
}

void debug_msg(const char * file_name, int line, const char * msg, int deb_level, ...) {
    if (debug_level != deb_level) {
        return;
    }

    va_list args;
    va_start(args, deb_level);

    char buffer[MEDIUM_SIZE], buffer2[LARGE_SIZE];
    if (show_origin) {
        snprintf(buffer, MEDIUM_SIZE, "\n--FILE %s, LINE %d--\n", file_name, line);
    }
    vsnprintf(buffer2, LARGE_SIZE, msg, args);

    if (show_origin) {
        fprintf(stdout, "%s%s\n", buffer, buffer2);
    } else {
        fprintf(stdout, "%s\n", buffer2);
    }

    va_end(args);
}

// endregion public functions