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
#include <float.h>
#include <assert.h>
#include <errno.h>
#include "to_float.h"

// endregion dependencies

// region public functions

int to_float(char * num_string, float * num_float){
    assert(num_string != NULL && num_float != NULL);

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

// endregion public functions
