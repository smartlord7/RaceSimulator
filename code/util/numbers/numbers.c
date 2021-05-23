/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

//region dependencies

#include "time.h"
#include "errno.h"
#include "stdlib.h"
#include "numbers.h"

//endregion dependencies

int random_int(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int random_uniform_event(float probability) {
    int num_chosen = (int) (probability * NUM_EVENT_VALUES),
        pivot = random_int(0, NUM_EVENT_VALUES);

    return num_chosen > pivot;
}
