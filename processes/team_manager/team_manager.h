/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

#ifndef RACESIMULATOR_C_TEAM_MANAGER_H
#define RACESIMULATOR_C_TEAM_MANAGER_H

// region constants

#define MIN_FUEL_LAPS 2
#define TEAM_MANAGER "TEAM_MANAGER"
#define RACE_TEAM "RACE_TEAM"
#define CAR_THREAD "CAR_THREAD"
#define CAR_THREADS "CAR_THREADS"
#define TEAM_CARS "TEAM_RACE_CARS"

// endregion constants

// region public functions

/**
 * @def team_manager
 * @brief Worker function executed when the team manager process is created.
 *
 */
void team_manager();

// endregion public functions

#endif //RACESIMULATOR_C_TEAM_MANAGER_H