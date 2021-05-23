/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
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
#define COMMANDS_HELP "------------------------------------RACE SIMULATOR------------------------------------\n"\
                      "AVAILABLE COMMANDS:\n"\
                      "-> 'START RACE!' - START THE RACE WITH THE GIVEN CONFIGURATIONS AND RACE CARS\n"\
                      "-> 'EXIT' - EXIT THE PROGRAM (EQUIVALENT TO Ctrl+Z)\n"\
                      "-> 'ADDCAR TEAM: <team_name>, CAR: <car_name>, SPEED: <speed>, CONSUMPTION: <consumption>, RELIABILITY. <reliability>' -\n" \
                      "ADD A CAR TO THE RACE WITH SPECIFIED PARAMETERS\n"\
                      "---------------------------------------------------------------------------------------\n"\
                      "CAR PARAMETER CONSTRAINTS:"\
                      "-> TEAM - MUST BE AN UNIQUE STRING AMONGST THE PREVIOUSLY INSERTED TEAM NAMES\n"\
                      "-> CAR - MUST BE AN UNIQUE STRING AMONGST THE PREVIOUSLY INSERTED CAR NAMES\n"\
                      "-> SPEED - MUST BE AN FLOATING POINT NUMBER GREATER THAN 0\n"\
                      "-> CONSUMPTION - MUST BE AN FLOATING POINT NUMBER GREATER THAN 0\n"\
                      "-> RELIABILITY - MUST BE AN FLOATING POINT NUMBER BETWEEN 0 (EXCLUSIVE) AND 100 (INCLUSIVE)\n"

extern int unn_pipe_fds[2];

// endregion constants

// region public functions prototypes

/**
 * @def race_manager
 * @brief Worker function executed when the race manager process is created.
 *
 */
extern void race_manager();

/**
 * @def create_new_team
 * @brief Function that creates a new race team.
 *
 * @param team_name
 * Name to be assigned to the team.
 *
 * @param team_id
 * Used to store the ID assigned to the team.
 *
 */
extern void create_new_team(char * team_name, int * team_id);

// endregion public functions prototypes

#endif //RACESIMULATOR_C_RACE_MANAGER_H
