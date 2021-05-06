/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../exception_handler/exception_handler.h"

// endregion dependencies

// region constants

#define STRING "STRING"
#define true 1
#define false 0

// endregion constants

// region public functions

char * string(int size) {
    assert(size > 0);

    char * buffer = NULL;

    if ((buffer = malloc(size * sizeof(char))) == NULL) {
        throw_and_exit(MEMORY_ALLOCATE_EXCEPTION, STRING);
    }

    return buffer;
}

char * append(char * first, char * second) {
    assert(first != NULL && second != NULL);

    if (strcat(first, second) == NULL) {
        throw_and_exit(STRCAT_EXCEPTION, "");
    }

    return first;
}

// endregion public functions