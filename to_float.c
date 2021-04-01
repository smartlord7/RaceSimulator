#include <errno.h>
#include <float.h>
#include <stdlib.h>
#include "error_handler.h"

int to_float(char num_string[], float * num_float){
    double num_d;
    char * tail;
    errno = 0;

    num_d = strtodf(num_string, &tail);

    if (errno) {
        return 0;
    }

    if (num_d <= FLT_MAX && num_d >= FLT_MIN) {
        * num_float = (float) num_d;
    } else {
        return -1;
    }

    return 1;
}
