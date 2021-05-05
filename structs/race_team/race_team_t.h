/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACESIMULATOR_C_RACE_TEAM_H
#define RACESIMULATOR_C_RACE_TEAM_H

// region dependencies

#include "../race_box/race_box_t.h"

// endregion dependencies

// region constants

#define MAX_TEAM_NAME_SIZE 50
#define MAX_MAX_CARS_PER_TEAM 20

// endregion constants

// region forward declarations

typedef struct race_team_t race_team_t;
typedef struct race_car_t race_car_t;
typedef struct race_box_t race_box_t;

// endregion forward declarations

// region structures

/**
 * @struct race_team_t
 * @brief Struct that represents a race team.
 *
 * @var race_team_t::team_name
 * The name of the race team.
 *
 * @var race_team_t::num_cars
 * The number of cars of the race team.
 *
 * @var race_team_t::team_box
 * A pointer to the box owned by the race team (acts as a navigation property).
 *
 * @var race_team_t::cars
 * An array of pointers to the cars owned by the race team (acts as an array of navigation properties).
 */
struct race_team_t {
    char team_name[MAX_TEAM_NAME_SIZE];
    int num_cars;
    race_box_t * team_box;
};

// endregion structures

/**
 * @def race_team
 * @brief Function that creates a new race team (constructor)
 *
 * @param team_name
 * The name of the race team.
 *
 * @return Pointer to the team.
 *
 * @throws MemoryAllocationException if the malloc call for the race car returns a NULL pointer.
 */
race_team_t * race_team(const char * team_name);

/**
 * @def race_team_to_string
 * @brief Function that retrieves a string representation of a race team.
 *
 * @param team
 * The race team.
 *
 * @return a string holding the race team information.
 *
 * @throws MemoryAllocationException if the malloc call for the string returns a NULL pointer.
 *
 */
char * race_team_to_string(race_team_t * team);

#endif //RACESIMULATOR_C_RACE_TEAM_H