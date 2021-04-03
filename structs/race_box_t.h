#ifndef RACESIMULATOR_RACE_BOX_H
#define RACESIMULATOR_RACE_BOX_H

#include "race_team_t.h"
#include "race_car_t.h"

typedef struct race_team_t race_team_t;
typedef struct race_car_t race_car_t;
typedef struct race_box_t race_box_t;

#define BOX_FREE_STATE 0
#define BOX_RESERVED_STATE 1
#define BOX_OCCUPIED_STATE 2

struct race_box_t{
    int state;
    race_team_t * team;
    race_car_t * current_car;
};

#endif //RACESIMULATOR_RACE_BOX_H