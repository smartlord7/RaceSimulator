/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

#ifndef RACESIMULATOR_C_RACE_MANAGER_H
#define RACESIMULATOR_C_RACE_MANAGER_H

// region constants

#define RACE_MANAGER "RACE_MANAGER"
#define TEAMS "TEAMS"
#define BOXES "BOXES"
#define ADDCAR "ADDCAR"
#define START "START"

#define RESULT_NEW_TEAM 0
#define RESULT_NEW_CAR 1
#define RESULT_NOT_RECO 2
#define RESULT_INVALID_CAR_DATA 3
#define RESULT_INVALID_TEAM 4
#define RESULT_NOT_ALL_TEAMS 5
#define RESULT_RACE_STARTED 6

#define COMMAND_PREVIEW_SIZE 7

// endregion constants

// region public functions prototypes

/**
 * @def race_manager
 * @brief Worker function executed when the race manager process is created.
 *
 */
void race_manager();

// endregion public functions prototypes

#endif //RACESIMULATOR_C_RACE_MANAGER_H
