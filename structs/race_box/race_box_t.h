/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACESIMULATOR_C_RACE_BOX_H
#define RACESIMULATOR_C_RACE_BOX_H

// region dependencies

#include <semaphore.h>
#include "../race_team/race_team_t.h"
#include "../race_car/race_car_t.h"
#include "../../ipcs/sync/mutex/mutex.h"
#include "../../ipcs/sync/cond_var/cond_var.h"

// endregion dependencies

// region forward declarations

typedef enum box_state_t box_state_t;
typedef struct race_car_t race_car_t;
typedef struct race_team_t race_team_t;

// endregion forward declarations

// region structures

/**
 * @enum box_state_t
 * Enum that represents the allowed states of a race box.
 */
enum box_state_t {
    FREE = 0,
    RESERVED = 1,
    OCCUPIED = 2
};

/**
 * @struct race_box_t
 * @brief Struct that represents a race box.
 *
 * @var race_box_t::state
 * The current state of the race box.
 *
 * @var race_box_t::team
 * A pointer to the race team that owns the race box (acts as a navigation property).
 *
 * @var race_box_t::current_car
 * The race car that is currently in the race box (acts as a navigation property).
 *
 * @var available
 * A pthread mutex that allows only one car to use the box at a time.
 *
 * @var mutex
 * A pthread mutex used essentially with the box cond. var.
 *
 * @var cond
 * A pthread cond. var. used to wait for certain conditions to be satisfied.
 *
 * @var car_dispatched
 * Flag that specifies if the current car in the box has been handled.
 *
 */
typedef struct race_box_t{
    box_state_t state;
    race_team_t * team;
    race_car_t * current_car;
    mutex_t available, mutex;
    cond_t cond;
    int car_dispatched; // used in order to respect the following project constraint: the team manager (and only) must update the box state in the shm.
} race_box_t;

// TODO Constructor and toString method.

// endregion structures

#endif //RACESIMULATOR_C_RACE_BOX_H