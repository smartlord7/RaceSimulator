#include <errno.h>
#include <float.h>
#include <stdlib.h>

int to_float(char * num_string, float * num_float){
    double num_d;
    char * tail;
    errno = 0;

    num_d = strtod(num_string, &tail);

    if (errno) {
        return 0;
    }

    if (num_d <= FLT_MAX && num_d >= FLT_MIN) {
        * num_float = (float) num_d;
    } else if (num_d == 0.0){
        return 0;
    } else {
        return -1;
    }

    return 1;
}
