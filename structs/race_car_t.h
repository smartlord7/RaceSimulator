/* Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* Authors:
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* Date of creation: 31/03/2021
*/

#ifndef RACESIMULATOR_RACE_CAR_H
#define RACESIMULATOR_RACE_CAR_H

#include "race_team_t.h"

#define MAX_LABEL_SIZE 50

typedef enum car_state car_state
typedef struct race_car_t race_car_t;
typedef struct race_team_t race_team_t;

enum car_state {
   RACE, SAFETY, IN_BOX_, NON_FIT, FINISHED
};

/**
 * Implementation of the concept of race car.
 */
struct race_car_t{
    race_team_t * team;
    char name[MAX_LABEL_SIZE];
    car_state state;
    int car_id,
            completed_laps,
            num_box_stops,
            num_malfunctions,
            num_of_refuels;
    float remaining_fuel,
            consumption,
            current_consumption,
            speed,
            current_speed,
            current_pos,
            reliability;
};

/**
 * Create a new car and initialize its properties.
 *
 * @param team Team who sponsors the car.
 * @param car_id ID to assign to car.
 * @param consumption Consumption of fuel.
 * @param speed Car speed.
 * @param reliability Reliability of the car.
 * @param initial_fuel Initial fuel value.
 * @return Return pointer to new car.
 */
race_car_t * race_car(race_team_t * team, int car_id, float consumption, float speed, float reliability, float initial_fuel);

/**
 * Convert car's information to string.
 * @param car Car containing the informations.
 * @return String with car's information.
 */
char * race_car_to_string(race_car_t * car);

#endif //RACESIMULATOR_RACE_CAR_H