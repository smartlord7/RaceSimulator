#ifndef RACESIMULATOR_RACE_TEAM_H
#define RACESIMULATOR_RACE_TEAM_H

#include "race_box_t.h"

#define MAX_TEAM_NAME_SIZE 50

typedef struct race_team_t race_team_t;
typedef struct race_car_t race_car_t;
typedef struct race_box_t race_box_t;

struct race_team_t {
    char team_name[MAX_TEAM_NAME_SIZE];
    int num_of_cars;
    race_box_t * team_box;
    race_car_t * cars;
};

race_team_t * race_team(const char * team_name);
char * race_team_to_string(race_team_t * team);

#endif //RACESIMULATOR_RACE_TEAM_H