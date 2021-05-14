#include "time.h"
#include "errno.h"
#include "stdlib.h"
#include "numbers.h"

int random_int(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int random_uniform_event(float probability) {
    int num_chosen = (int) (probability * NUM_EVENT_VALUES),
        pivot = random_int(0, NUM_EVENT_VALUES);

    return num_chosen > pivot;
}

void ms_sleep(uint ms) {
    int result = 0;
    struct timespec ts_remaining = {ms / 1000, (ms % 1000) * 1000000L};

    do {
        struct timespec ts_sleep = ts_remaining;
        result = nanosleep(&ts_sleep, &ts_remaining);
    } while ((EINTR == errno) && (-1 == result));
}
