#include <stdio.h>
#include "string.h"
#include "assert.h"
#include "cmd_validator.h"
#include "../../util/global.h"
#include "../../util/strings/strings.h"
#include "../../util/to_float/to_float.h"
#include "../../processes/race_manager/race_manager.h"

static int check_start_conditions();
static int get_team_id_by_name(char * team_name);
static int validate_car(char * buffer, race_car_t * car, int * team_id);
static int validate_team(char * buffer, race_team_t * team);
static int validate_number(char * buffer, char * expected_cmd, float * result);
static int is_car_name_unique(char * car_name);

int num_registed_teams;

int interpret_command(char * buffer, race_car_t * car, int * team_id) {
    if (strcasecmp(buffer, START_RACE) == 0) {
        return check_start_conditions();
    } else if (starts_with_ignore_case(buffer, ADDCAR)) {
        buffer += strlen(ADDCAR) + 1;
        if (validate_car(buffer, car, team_id)) {
            return RESULT_NEW_CAR;
        } else {
            return RESULT_INVALID_CAR;
        }
    }
    return RESULT_INVALID_COMMAND;
}

static int check_start_conditions() {
    int i;

    for(i = 0; i < config.num_teams; i++) {
        if(shm->race_teams[i].num_cars == 0) {
            return RESULT_CANNOT_START_RACE;
        }
    }

    return RESULT_BEGIN_RACE;
}

static int get_team_id_by_name(char * team_name) {
    int i;

    for(i = 0; i < config.num_teams; i++) {
        if(shm->race_teams[i].team_id < 0) {
            return -1;
        }

        if(strcasecmp(shm->race_teams[i].team_name, team_name) == 0) {
            return shm->race_teams[i].team_id;
        }
    }

    return -1;
}

static int validate_car(char * buffer, race_car_t * car, int * team_id) {
    assert(buffer != NULL);
    float speed, consumption, reliability, car_number_f;
    char * token, car_name[LARGE_SIZE], aux[LARGEST_SIZE];
    int car_number;
    race_team_t team;

    // validate team name
    if((token = strtok_r(buffer, DELIM_1, &buffer)) == NULL) {
        return false;
    }
    strcpy(aux, token);
    if(!validate_team(aux, &team)) {
        return false;
    }

    // validate car number
    if((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        return false;
    }
    strcpy(aux, token);
    if(!validate_number(aux, CAR , &car_number_f)) {
        return false;
    }
    car_number = (int) car_number_f;
    snprintf(car_name, MAX_LABEL_SIZE, "%d", car_number);
    if(!is_car_name_unique(car_name)) {
        return false;
    }

    //validate speed
    if((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        return false;
    }
    strcpy(aux, token);
    if (!validate_number(aux, SPEED, &speed) || speed <= 0) {
        return false;
    }

    // validate consumption
    if((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        return false;
    }
    strcpy(aux, token);
    if(!validate_number(aux, CONSUMPTION, &consumption) || consumption <= 0)  {
        return false;
    }

    // validate reliability
    if((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        return false;
    }
    strcpy(aux, token);
    if(!validate_number(aux, RELIABILITY, &reliability) || reliability <= 0 || reliability > 100) {
        return false;
    }

    *team_id = team.team_id;
    *car = *race_car(&team, car_name, -1, consumption, speed, reliability/100, config.fuel_tank_capacity); // ATTENTION: I
    return true;
}

static int validate_team(char * buffer, race_team_t * team) {
    char * team_field, *team_name;
    int team_id;

    if ((team_field = strtok_r(buffer, DELIM_2, &buffer)) == NULL) return false;
    else {
        team_field = trim_string(team_field, (int) strlen(team_field));
        if (strcasecmp(team_field, TEAM) == 0) {

            if ((team_name = strtok_r(NULL, DELIM_2, &buffer)) == NULL) return false;
            team_name = trim_string(team_name, (int) strlen(team_name));

            team_id = get_team_id_by_name(team_name);
            if(team_id < 0 && num_registed_teams < config.num_teams) {
                create_team(team_name, &team_id);
            } else return false;
            *team = shm->race_teams[team_id];

            if(team->num_cars == config.max_cars_per_team) {
                return false;
            }

            return true;

        } else return false;
    }
}

static int validate_number(char * buffer, char * expected_cmd, float * result) {
    char * cmd_field, * value_field;
    float value;

    if((cmd_field = strtok_r(buffer, DELIM_2, &buffer)) == NULL) {
        return false;
    }
    cmd_field = trim_string(cmd_field, (int) strlen(cmd_field));

    if (!strcasecmp(cmd_field, expected_cmd)) {

        if((value_field = strtok_r(NULL, DELIM_2, &buffer)) == NULL) {
            return false;
        }

        value_field = trim_string(value_field, (int) strlen(value_field));

        if (to_float(value_field, &value) == FLOAT_CONVERSION_FAILURE || value <= 0) {
            return false;
        }

        * result = value;

        return true;

    }

    return false;
}

static int is_car_name_unique(char * car_name) {
    int i, j;
    race_car_t car_B;

    for(i = 0; i < config.num_teams; i++) {
        for(j = 0; j < shm->race_teams[i].num_cars; j++) {
            car_B = shm->race_cars[i][j];
            if(strcasecmp(car_B.name, car_name) == 0) {
                return false;
            }
        }
    }

    return true;
}