#include <errno.h>
#include <float.h>
#include <stdlib.h>

#define FLOAT_SIZE_EXCEEDED -1
#define FLOAT_CONVERSION_FAILURE 0
#define FLOAT_CONVERSION_SUCCESS 1

int to_float(char * num_string, float * num_float){
    double num_d;
    char * tail;
    errno = 0;

    num_d = strtod(num_string, &tail);

    if (errno) {
        return FLOAT_CONVERSION_FAILURE;
    }

    if (num_d <= FLT_MAX && num_d >= FLT_MIN) {
        * num_float = (float) num_d;
    } else if (num_d == 0.0){
        return FLOAT_CONVERSION_FAILURE;
    } else {
        return FLOAT_SIZE_EXCEEDED;
    }

    return FLOAT_CONVERSION_SUCCESS;
}
