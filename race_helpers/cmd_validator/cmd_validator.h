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

extern int interpret_command(char * buffer, race_car_t * car_data);
extern int check_start_conditions();
extern int validate_car(char * buffer, race_car_t * car);
extern int validate_team(char * buffer, race_team_t * team);
extern int validate_number(char * buffer, char * expected_cmd, float * result);
extern int is_car_id_unique(int car_id, race_team_t * team);

#endif //RACE_HELPERS_CMD_VALIDATOR_H
