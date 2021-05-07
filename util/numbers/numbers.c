#include "stdlib.h"
#include "numbers.h"

int random_int(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int random_uniform_event(float probability) {
    int num_chosen = (int) (probability * NUM_EVENT_VALUES),
        pivot = random_int(0, NUM_EVENT_VALUES);

    return num_chosen >= pivot;
}