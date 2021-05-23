/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef STRUCTS_RACE_TEAM_H
#define STRUCTS_RACE_TEAM_H

// region dependencies

#include "../race_box/race_box_t.h"
#include "../../ipcs/sync/mutex/mutex.h"

// endregion dependencies

// region constants

#define true 1
#define false 0
#define NULL_STR_SIZE 5
#define RACE_TEAM "RACE TEAM"
#define MAX_TEAM_NAME_SIZE 50

// endregion constants

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
typedef struct race_team_t {
    int team_id, num_cars, num_cars_safety;
    char team_name[MAX_TEAM_NAME_SIZE];
    race_box_t team_box;
    mutex_t pipe_mutex;
} race_team_t;

// endregion structures

//region functions prototypes

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
extern race_team_t * race_team(int team_id, const char * team_name);

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
extern char * race_team_to_string(race_team_t * team);

/**
 * @def race_box_state_to_string
 * @brief Function that converts the state of the team's box to a legible string.
 *
 * @param state
 * The box state.
 *
 * @return A string representation of the box state.
 */
extern char * race_box_state_to_string(box_state_t state);

// endregion functions prototypes

#endif //STRUCTS_RACE_TEAM_H