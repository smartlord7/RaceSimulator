#ifndef STRUCTS_RACE_CAR_STATE_CHANGE_T_H
#define STRUCTS_RACE_CAR_STATE_CHANGE_T_H

#include "../race_car/race_car_t.h"

typedef struct race_car_state_change_t {
    int car_id, prev_state, new_state, malfunctioning : 1;
} race_car_state_change_t;

#endif //STRUCTS_RACE_CAR_STATE_CHANGE_T_H
