#ifndef RACESIMULATOR_RACE_CAR_H
#define RACESIMULATOR_RACE_CAR_H

#define CAR_RACE_STATE 0
#define CAR_SAFETY_STATE 1
#define CAR_BOX_STATE 2
#define CAR_NON_FIT_STATE 3
#define CAR_FINISHED_STATE 4

#include "race_team_t.h"

typedef struct race_car_t race_car_t;
typedef struct race_team_t race_team_t;

struct race_car_t{
    race_team_t * team;
    int car_id,
            completed_laps,
            num_box_stops,
            num_malfunctions,
            num_of_refuels,
            status;
    float remaining_fuel,
            consumption,
            current_consumption,
            speed,
            current_speed,
            current_pos,
            reliability;
};

race_car_t * race_car(race_team_t * team, int car_id, float consumption, float speed, float reliability);
char * race_car_to_string(race_car_t * car);

#endif //RACESIMULATOR_RACE_CAR_H
