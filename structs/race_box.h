#ifndef RACESIMULATOR_RACE_BOX_H
#define RACESIMULATOR_RACE_BOX_H

#include "race_team.h"

#define BOX_FREE_STATE 0
#define BOX_RESERVED_STATE 1
#define BOX_OCCUPIED_STATE 2

typedef struct race_box race_box;

struct race_box{
    int state;
    race_team * team;
    race_car * current_car;
};

#endif //RACESIMULATOR_RACE_BOX_H
