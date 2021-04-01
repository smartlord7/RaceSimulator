#ifndef RACESIMULATOR_RACE_TEAM_H
#define RACESIMULATOR_RACE_TEAM_H

#include "race_car.h"
#include "race_box.h"

#define MAX_TEAM_NAME_SIZE 20

typedef struct race_team race_team;

struct race_team {
    char team_name[MAX_TEAM_NAME_SIZE];
    int num_of_cars;
    race_box * team_box;
    race_car * cars;
};

#endif //RACESIMULATOR_RACE_TEAM_H
