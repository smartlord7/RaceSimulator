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

#include "race_config_t.h"
#include "race_car_t.h"

// endregion dependencies

// region forward declarations

typedef struct shared_memory_t shared_memory_t;

// endregion forward declarations

// region structures

/**
 * @struct shared_memory_t
 * @brief Struct that holds all the information needed to be shared amongst the created processes.
 *
 * @var shared_memory_t::cfg
 * The race configuration.
 *
 * @var shared_memory_t::race_teams
 * The teams participating in the race.
 *
 * @var shared_memory_t::race_cars
 * The cars running in the race.
 *
 */
struct shared_memory_t {
    race_config_t * cfg;
    race_team_t * race_teams;
    race_car_t * race_cars;
    /*int cars_on_track,
        total_num_cars,    TODO: Integrate in a stats struct.
        num_malfunctions,
        num_refuels;*/
};

// endregion structures

#endif //RACESIMULATOR_C_SHARED_MEMORY_H