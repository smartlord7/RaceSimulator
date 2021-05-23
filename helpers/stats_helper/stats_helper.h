/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef RACE_HELPERS_STATS_HELPER_H
#define RACE_HELPERS_STATS_HELPER_H

//region dependencies

#include "../../structs/race_config/race_config_t.h"
#include "../../structs/shared_memory/shared_memory_t.h"

//endregion dependencies

//region constants

// region table columns

#define RACE_STATISTICS "RACE STATISTICS"
#define CAR_RACE_POS "#"
#define CAR_ID "ID"
#define CAR_TEAM_ID "TEAM ID"
#define CAR_NAME "CAR"
#define CAR_TEAM_NAME "TEAM NAME"
#define CAR_STATE "STATE"
#define CAR_NUM_COMPLETED_LAPS "LAPS"
#define CAR_NUM_BOX_STOPS "BOX STOPS"
#define CAR_POS "POSITION (m)"
#define CAR_FINISH_TIME "FINISH TIME (tu)"
#define RACE_NUM_MALFUNCTIONS "MALFUNCTIONS: "
#define RACE_NUM_REFUELS "REFUELS: "
#define RACE_NUM_CARS_ON_TRACK "CARS ON TRACK: "

// endregion table columns

#define NUM_TOP_CARS 5
#define MAX_DIGITS 3
#define NUM_COLS 7
#define MAX_STATE_LENGTH 12
#define HORIZONTAL_DELIM "-"

// endregion constants

// region public function prototypes

/**
 * @def stats_helper_init
 * @brief Function that initializes the functionality to capture race statistics.
 *
 * @param cfg
 * The race configuration.
 *
 * @param shmem
 * Shared memory containing the race's data.
 *
 * @param mtx
 * pthread mutex associated stats capture synchronization.
 *
 */
extern void stats_helper_init(race_config_t * cfg, shared_memory_t * shmem, mutex_t * mtx);

/**
 * @def show_stats_table
 * @brief Function that shows the statistics table about the race.
 *
 */
extern void show_stats_table();

// endregion public function prototypes

// region global variables

extern race_config_t * conf;
extern shared_memory_t * sha_mem;
extern mutex_t * mutex;

// endregion global variables

#endif //RACE_HELPERS_STATS_HELPER_H