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

#define REFUEL_MIN_LAPS1 4
#define REFUEL_MIN_LAPS2 2
#define TEAM_MANAGER "" "TEAM_MANAGER"
#define RACE_CAR_SAY "[RACE_CAR %d] "
#define CAR_MOVE RACE_CAR_SAY "" "RAN %.2fm!"
#define CAR_FINISH RACE_CAR_SAY "" "FINISHED THE RACE!"
#define CAR_STATE_CHANGE RACE_CAR_SAY "" "CHANGED TO STATE %d!"
#define CAR_COMPLETE_LAP RACE_CAR_SAY "" "COMPLETED %d LAP!"
#define GLOBAL_CLOCK "[GLOBAL_CLOCK] %d tu"

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