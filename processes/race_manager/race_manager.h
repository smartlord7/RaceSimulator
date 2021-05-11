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

#define DELIM_1 ","
#define DELIM_2 ":"

#define RACE_MANAGER "RACE_MANAGER"
#define ADDCAR "ADDCAR"
#define CAR "CAR"
#define TEAM "TEAM"
#define SPEED "SPEED"
#define CONSUMPTION "CONSUMPTION"
#define RELIABILITY "RELIABILITY"
#define START_RACE "START RACE!"
#define COMMAND_REJECT "COMMAND REJECTED! THE RACE HAS ALREADY STARTED!"
#define RACE_MANAGER_SAY "[RACE MANAGER] "
#define CAR_STATE_CHANGE RACE_MANAGER_SAY "CAR %d CHANGED ITS STATE TO %d!"
#define CARS_FINISH "ALL CARS HAVE CROSSED THE FINISH LINE!"

#define RESULT_NOT_RECO 0
#define RESULT_INVALID_COMMAND 1
#define RESULT_NEW_CAR 2
#define RESULT_INVALID_CAR 3
#define RESULT_BEGIN_RACE 4
#define RESULT_CANNOT_START_RACE 5
#define RESULT_RACE_ALREADY_STARTED 6

#define COMMAND_PREVIEW_SIZE 7

extern int unn_pipe_fds[2];

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
