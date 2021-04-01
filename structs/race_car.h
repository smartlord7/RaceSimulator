#ifndef RACESIMULATOR_RACE_CAR_H
#define RACESIMULATOR_RACE_CAR_H

#include "race_team.h"

#define CAR_RACE_STATE 0
#define CAR_SAFETY_STATE 1
#define CAR_BOX_STATE 2
#define CAR_NON_FIT_STATE 3
#define CAR_FINISHED_STATE 4

typedef struct race_car race_car;

struct race_car{
    race_team * team;
    int car_id,
            completed_laps,
            num__box_stops,
            num_malfunctions,
            num_of_refuels,
            status;
    float remaining_fuel,
            consumption,
            speed,
            reliability;
};

#endif //RACESIMULATOR_RACE_CAR_H
