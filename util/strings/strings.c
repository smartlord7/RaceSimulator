/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "ctype.h"
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

char * trim_string(char * buffer, int size){
    assert(buffer != NULL && size > 0);

    int i, j;

    // cleanup any white spaces before
    i = 0;
    while (i < size && isspace(*buffer)){
        i++;
        buffer++;
    }

    // cleanup any whitespace after
    j = 0;
    while (i < size && !isspace(buffer[j])){
        j++;
        i++;
    }
    buffer[j] = '\0';

    return buffer;
}

int starts_with(char * string, char * string2) {
    assert(string != NULL && string2 != NULL);

    char * i, * j;

    i = string;
    j = string2;

    while ((* j) != '\0') {
        if ((* i++) != (* j++)) {
            return false;
        }
    }

    return true;
}

int starts_with_ignore_case(char * string, char * string2) {
    assert(string != NULL && string2 != NULL);

    char * i, * j;

    i = string;
    j = string2;

    while ((* j) != '\0') {
        if (toupper((int) * i++) != toupper((int) (* j++))) {
            return false;
        }
    }

    return true;
}

int get_max_str_length(char ** strings, int size) {
    assert(strings != NULL && size > 0);

    int max_len = -1, i = 0, len;

    while (i < size) {
        len = (int) strlen(strings[i]);

        if (len > max_len) {
            max_len = len;
        }

        i++;
    }

    return max_len;
}

char * repeat_str(char * str, int times) {
    assert(str != NULL && times > 0);

    char * buffer;
    int i;

    buffer = string(times);
    strcpy(buffer, str);
    i = 0;

    while (i < times - 1) {
        buffer = strcat(buffer, str);

        i++;
    }

    return buffer;
}

// endregion public functions