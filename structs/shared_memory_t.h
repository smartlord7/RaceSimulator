/* Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* Authors:
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* Date of creation: 31/03/2021
*/

#ifndef RACESIMULATOR_SHARED_MEMORY_H
#define RACESIMULATOR_SHARED_MEMORY_H

#include "race_config_t.h"
#include "race_car_t.h"
#include "pthread.h"

typedef struct shared_memory_t shared_memory_t;

/**
 * Implementation of the shared memory zone.
 */
struct shared_memory_t {
    race_config_t * cfg;
    race_team_t * race_teams;
    race_car_t * race_cars;
    race_box_t * boxes;
    int cars_on_track, total_num_cars;
    pid_t * child_processes_ids;
};

#endif //RACESIMULATOR_SHARED_MEMORY_H