/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef RACE_HELPERS_CMD_VALIDATOR_H
#define RACE_HELPERS_CMD_VALIDATOR_H

// region dependencies

#include "../../structs/race_car/race_car_t.h"

// endregion dependencies


// region constants

#define DELIM_1 ","
#define DELIM_2 ":"

// region cmd parsing results

#define RESULT_INVALID_COMMAND 1
#define RESULT_NEW_CAR 2
#define RESULT_INVALID_CAR 3
#define RESULT_BEGIN_RACE 4
#define RESULT_CANNOT_START_RACE 5
#define RESULT_EXIT 6
#define RESULT_HELP 7

// endregion cmd parsing results

// region car properties

#define CAR "CAR"
#define TEAM "TEAM"
#define SPEED "SPEED"
#define CONSUMPTION "CONSUMPTION"
#define RELIABILITY "RELIABILITY"

// endregion car properties

// region available commands

#define START_RACE "START RACE!"
#define ADDCAR "ADDCAR"
#define EXIT_SIMULATION "EXIT"
#define HELP_REQ "HELP"

// endregion available commands

// region car/team parsing results

#define NEW_TEAM (-1)
#define NO_TEAM_SLOT (-2)
#define MISSING_ATTR (-3)
#define NO_CAR_SLOT (-4)

// endregion car/team parsing results


// endregion constants

// region public functions prototypes

/**
 * @def interpret_command
 * @brief Function that interprets the meaning of a given command.
 *
 * @param buffer
 * Command to be interpreted.
 *
 * @param car
 * Car buffer that will holde the car data if the command is ADDCAR and is valid.
 *
 * @param team_id
 * Internal team ID that owns the car if the command is to add a new car and is valid.
 *
 * @return RESULT_CANNOT_START_RACE if the command is to start the race and the last one does not fill the required conditions to start.
 *         RESULT_BEGIN_RACE if the command is to start the race and this one fills all of the required conditions to be started.
 *         RESULT_NEW_CAR if the command is to add new car to the race and all of the data is valid.
 *         RESULT_INVALID_CAR if the command is to add a new car but the data is not valid.
 *         RESULT_INVALID_COMMAND if the command is not recognized as one of the options above.
 */
extern int interpret_command(char * buffer, race_car_t * car, int * team_id);

// endregion public functions prototypes

// region global variables

extern int num_registered_teams;

// endregion global variables

#endif //RACE_HELPERS_CMD_VALIDATOR_H
