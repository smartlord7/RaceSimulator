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
#define DELIM_3 "\n"

#define RACE_MANAGER "RACE_MANAGER"
#define RACE_MANAGER_SAY "[RACE MANAGER] "
#define CAR_CHANGE RACE_MANAGER_SAY "CAR %d CHANGED ITS STATE TO %d!"
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
extern void race_manager();

extern void create_new_team(char * team_name, int * team_id);

// endregion public functions prototypes

#endif //RACESIMULATOR_C_RACE_MANAGER_H
