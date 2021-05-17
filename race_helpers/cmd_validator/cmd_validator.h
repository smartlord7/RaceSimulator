#ifndef RACE_HELPERS_CMD_VALIDATOR_H
#define RACE_HELPERS_CMD_VALIDATOR_H

#include "../../structs/race_car/race_car_t.h"

#define DELIM_1 ","
#define DELIM_2 ":"
#define ADDCAR "ADDCAR"

#define RESULT_INVALID_COMMAND 1
#define RESULT_NEW_CAR 2
#define RESULT_INVALID_CAR 3
#define RESULT_BEGIN_RACE 4
#define RESULT_CANNOT_START_RACE 5

#define CAR "CAR"
#define TEAM "TEAM"
#define SPEED "SPEED"
#define CONSUMPTION "CONSUMPTION"
#define RELIABILITY "RELIABILITY"
#define START_RACE "START RACE!"

extern int interpret_command(char * buffer, race_car_t * car, int * team_id);

extern int num_registered_teams;

#endif //RACE_HELPERS_CMD_VALIDATOR_H
