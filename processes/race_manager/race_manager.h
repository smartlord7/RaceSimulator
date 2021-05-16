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
#define RACE_MANAGER_SAY "[RACE MANAGER] "
#define COMMAND_REJECT "COMMAND REJECTED! THE RACE HAS ALREADY STARTED!"
#define CAR_STATE_CHANGE RACE_MANAGER_SAY "CAR %d CHANGED ITS STATE TO %d!"
#define RACE_END "THE RACE HAS ENDED!"
#define CAR_WIN "THE CAR %d FROM TEAM %d HAS WON THE RACE AT %d tu!"

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
