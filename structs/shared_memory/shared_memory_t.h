/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACESIMULATOR_C_SHARED_MEMORY_H
#define RACESIMULATOR_C_SHARED_MEMORY_H

// region dependencies

#include "../race_car/race_car_t.h"
#include "../race_config/race_config_t.h"
#include "../sync_t/sync_t.h"

// endregion dependencies

// region constants

#define MAX_NUM_TEAMS 15 // TODO: move constraints to a separate file
#define MAX_MAX_CARS_PER_TEAM 100

// endregion constants

// region structures

/**
 * @struct shared_memory_t
 * @brief Struct that holds all the information needed to be shared amongst the created processes of the race simulator.
 *
 *
 * @var shared_memory_t::race_teams
 * The teams participating in the race.
 *
 * @var shared_memory_t::race_cars
 * The cars running in the race.
 *
 * @var shared_memory_t::race_boxes
 * The teams boxes.
 *
 */
typedef struct shared_memory_t {
    race_team_t race_teams[MAX_NUM_TEAMS];
    race_car_t race_cars[MAX_NUM_TEAMS][MAX_MAX_CARS_PER_TEAM];
    sync_t sync_s;
    int num_cars_on_track,
        total_num_cars,
        num_finished_cars, // TODO: Integrate in a stats struct.
        num_malfunctions,
        num_refuels;
} shared_memory_t;

// endregion structures

#endif //RACESIMULATOR_C_SHARED_MEMORY_H