#include "string.h"
#include "assert.h"
#include "cmd_validator.h"
#include "../../util/global.h"
#include "../../util/strings/strings.h"
#include "../../util/to_float/to_float.h"

int interpret_command(char * buffer, race_car_t * car) {
    if (strcasecmp(buffer, START_RACE) == 0) {
        return check_start_conditions();
    } else if (starts_with_ignore_case(buffer, ADDCAR)) {
        buffer += strlen(ADDCAR);
        if (validate_car(buffer, car)) {
            return RESULT_NEW_CAR;
        } else {
            return RESULT_INVALID_CAR;
        }
    }
    return RESULT_INVALID_COMMAND;
}

int check_start_conditions() {
    int i;

    for(i = 0; i < config.num_teams; i++) {
        if (shm->race_teams[i].num_cars == 0) return RESULT_CANNOT_START_RACE;
    }

    return RESULT_BEGIN_RACE;
}

race_team_t * get_team_by_name(char * team_name) {
    int i;

    for(i = 0; i < config.num_teams && shm->race_teams[i].team_id > 0; i++) {

        if (strcasecmp(shm->race_teams[i].team_name, team_name) == 0) {
            return &shm->race_teams[i];
        }
    }

    return NULL;
}

int validate_car(char * buffer, race_car_t * car) {
    assert(buffer != NULL);
    float speed, consumption, reliability, car_id_f;
    char * token;
    int car_id;
    race_team_t team;

    // validate team name.
    if ((token = strtok(buffer, DELIM_1)) == NULL) {
        return false;
    }
    if (!validate_team(token, &team)) {
        return false;
    }

    // validate car id.
    if ((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }

    // validate car.
    if (!validate_number(token, CAR , &car_id_f)) {
        return false;
    }
    car_id = (int) car_id_f;
    if (!is_car_id_unique(car_id, &team)) {
        return false;
    }

    //validate speed
    if ((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if (!validate_number(token, SPEED, &speed) || speed <= 0) {
        return false;
    }

    // validate consumption
    if ((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if (!validate_number(token, CONSUMPTION, &consumption) || consumption <= 0)  {
        return false;
    }

    // validate reliability
    if ((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if (validate_number(token, RELIABILITY, &reliability) || reliability <= 0) {
        return false;
    }

    *car = *race_car(&team, car_id, consumption, speed, reliability, config.fuel_tank_capacity); // ATTENTION: I

    return true;
}

int validate_team(char * buffer, race_team_t * team) {
    char * team_field, *team_name;

    if ((team_field = strtok(buffer, DELIM_2)) == NULL) return false;
    else {
        team_field = trim_string(team_field, (int) strlen(team_field));
        if (strcasecmp(team_field, TEAM) == 0) {

            if ((team_name = strtok(NULL, DELIM_2)) == NULL) return false;
            team_name = trim_string(team_name, (int) strlen(team_name));

            team = get_team_by_name(team_name);

            if (team == NULL || team->num_cars == config.max_cars_per_team) {
                return false;
            }

            return true;

        } else return false;
    }
}

int validate_number(char * buffer, char * expected_cmd, float * result) {
    char * cmd_field, * value_field;
    float value;

    if ((cmd_field = strtok(buffer, DELIM_2)) == NULL) {
        return false;
    }

    if (!strcasecmp(cmd_field, expected_cmd)) {

        if ((value_field = strtok(NULL, DELIM_2)) == NULL) {
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

int is_car_id_unique(int car_id, race_team_t * team) {
    int i, j;
    race_car_t car_B;

    for(i = 0; i < MAX_NUM_TEAMS; i++) {
        for(j = 0; j < MAX_MAX_CARS_PER_TEAM; j++) {
            car_B = shm->race_cars[i][j];

            if (car_B.team->team_id == team->team_id && car_B.car_id == car_id) {
                return false;
            }
        }
    }

    return true;
}
