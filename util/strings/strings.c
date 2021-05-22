/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "ctype.h"
#include "../exception_handler/exception_handler.h"
#include "strings.h"

// endregion dependencies

// region constants

#define STRING "STRING"
#define true 1
#define false 0

// endregion constants

//region private functions prototypes

/**
 * @def get_max_str_length
 * @brief Function that obtain the maximum string length from a set of strings.
 *
 * @param strings
 * Set of strings.
 *
 * @param size
 * Size of the set of strings.
 *
 * @return the maximum length found.
 */
static int get_max_str_length(char ** strings, int size);

//endregion private functions prototypes

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

char * append_f(char * first, const char * fmt, ...) {
    assert(first != NULL && fmt != NULL);

    char buffer[BUF_SIZE];

    va_list args;
    va_start(args, fmt);

    vsnprintf(buffer, BUF_SIZE, fmt, args);
    append(first, buffer);

    va_end(args);

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

char * repeat_str(const char * str, int times) {
    assert(str != NULL && times > 0);

    char * buffer;
    int i;

    buffer = string(times + 1);
    strcpy(buffer, str);
    i = 0;

    while (i < times - 2) {
         strcat(buffer, str);

        i++;
    }

    buffer = strcat(buffer, str);

    return buffer;
}

int is_number(const char * str) {
    int i = 0, is_decimal = false;
    char c;

    if (str[0] == '-') {
        i++;
    }

    while ((c = str[i++]) != '\0') {
        if (!is_decimal && c == '.') {
            is_decimal = true;
        } else if (!isdigit((int) c)) {
            return false;
        }
    }

    return true;
}

// endregion public functions