/*
 * Authors:
 *  - Joao Filipe Guiomar Artur, 2019217853
 *  - Sancho Amaral Simoes, 2019217590
 *
 * Date of creation: 02/04/2021
 */

#ifndef RACESIMULATOR_RACE_TEAM_H
#define RACESIMULATOR_RACE_TEAM_H

#include "race_box_t.h"

#define MAX_TEAM_NAME_SIZE 50

typedef struct race_team_t race_team_t;
typedef struct race_car_t race_car_t;
typedef struct race_box_t race_box_t;

/**
 * Implement the concept of race team.
 */
struct race_team_t {
    char team_name[MAX_TEAM_NAME_SIZE];
    int num_of_cars;
    race_box_t * team_box;
    race_car_t * cars;
};

/**
 * Create a new race team.
 * @param team_name Name to be assigned to team.
 * @return Pointer to the team.
 */
race_team_t * race_team(const char * team_name);

/**
 * Convert team's info to string.
 *
 * @param team Team whose info is to be converted.
 * @return String with team's information.
 */
char * race_team_to_string(race_team_t * team);

#endif //RACESIMULATOR_RACE_TEAM_H