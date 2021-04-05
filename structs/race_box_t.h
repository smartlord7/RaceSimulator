/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACESIMULATOR_C_RACE_BOX_H
#define RACESIMULATOR_C_RACE_BOX_H

// region dependencies

#include "race_team_t.h"
#include "race_car_t.h"

// endregion dependencies

// region forward declarations

typedef enum box_state box_state;
typedef struct race_team_t race_team_t;
typedef struct race_car_t race_car_t;
typedef struct race_box_t race_box_t;

// endregion forward declarations

// region structures
/**
 * @enum box_state
 * Enum that represents the allowed states of a race box.
 */
enum box_state {
    FREE, RESERVED, OCCUPIED
};

/**
 * @struct race_box_t
 * @brief Struct that represents a race box.
 *
 * @var race_box_t::state
 * The current state of the race box.
 *
 * @var race_box_t::team
 * The race team that owns the race box (acts as a navigation property).
 *
 * @var race_box_t::current_car
 * The race car that is currently in the race box (acts as a navigation property).
 */
struct race_box_t{
    box_state state;
    race_team_t * team;
    race_car_t * current_car;
};

// endregion structures

#endif //RACESIMULATOR_C_RACE_BOX_H