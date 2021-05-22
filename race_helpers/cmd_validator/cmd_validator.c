#include <malloc.h>
#include "string.h"
#include "assert.h"
#include "cmd_validator.h"
#include "../../util/global.h"
#include "../../util/strings/strings.h"
#include "../../util/to_float/to_float.h"
#include "../../processes/race_manager/race_manager.h"
#include "../log_generator/log_generator.h"

static int check_start_conditions();
static int get_team_id_by_name(char * team_name);
static int validate_car(char * buffer, race_car_t * car, int * team_id);
static int validate_team(char * buffer, race_team_t * team, int * return_flag);
static int validate_number_attr(char * buffer, const char * expected_attr, float * result);
static int is_car_name_unique(char * car_name);
static int validate_string_attr(char * buffer, const char * expected_attr, char * result);

int num_registered_teams;

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
    } else if (strcasecmp(buffer, EXIT_SIMULATION) == 0) {
        return RESULT_EXIT;
    }
    return RESULT_INVALID_COMMAND;
}

static int check_start_conditions() {
    int i;

    for(i = 0; i < config.num_teams; i++) {
        if (shm->race_teams[i].num_cars == 0) {
            generate_log_entry(ERROR_NOT_ENOUGH_TEAMS, NULL, NULL);
            return RESULT_CANNOT_START_RACE;
        }
    }

    return RESULT_BEGIN_RACE;
}

static int get_team_id_by_name(char * team_name) {
    int i;

    for(i = 0; i < config.num_teams; i++) {
        //belongs to a already registered team
        if (strcasecmp(shm->race_teams[i].team_name, team_name) == 0) {
            return shm->race_teams[i].team_id;
        }
        //belongs to a new team
        if (shm->race_teams[i].team_id < 0) {
            return NEW_TEAM;
        }
    }

    //does not belong to any registered team and there are no more slots for new teams
    return NO_TEAM_SLOT;
}

static int validate_car(char * buffer, race_car_t * car, int * team_id) {
    assert(buffer != NULL);
    float speed, consumption, reliability;
    char * token, car_name[LARGE_SIZE], aux[LARGEST_SIZE];
    race_team_t team;
    race_car_t * temp = NULL;
    int return_flag;

    // validate team name
    if ((token = strtok_r(buffer, DELIM_1, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) TEAM, NULL);
        return false;
    }
    strcpy(aux, token);
    if (!validate_team(aux, &team, &return_flag)) {
        if(return_flag == MISSING_ATTR) {
            generate_log_entry(ERROR_MISSING_CAR_ATTR, (void *) TEAM, NULL);
        }
        return false;
    }

    // validate car name
    if ((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) CAR, NULL);
        return false;
    }
    strcpy(aux, token);
    if (!validate_string_attr(aux, CAR, car_name))  {
        return false;
    }
    if (!is_car_name_unique(car_name)) {
        generate_log_entry(ERROR_UNIQUE_CONSTRAINT_VIOLATED, (void *) CAR, car_name);
        return false;
    }

    //validate speed
    if ((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) SPEED, NULL);
        return false;
    }
    strcpy(aux, token);
    if (!validate_number_attr(aux, SPEED, &speed) || speed <= 0) {
        return false;
    }

    // validate consumption
    if ((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) CONSUMPTION, NULL);
        return false;
    }
    strcpy(aux, token);
    if (!validate_number_attr(aux, CONSUMPTION, &consumption) || consumption <= 0)  {
        return false;
    }

    // validate reliability
    if ((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) RELIABILITY, NULL);
        return false;
    }
    strcpy(aux, token);
    if (!validate_number_attr(aux, RELIABILITY, &reliability) || reliability < 0 || reliability > 100) {
        return false;
    }

    * team_id = team.team_id;
    temp = race_car(&team, car_name, -1, consumption, speed, reliability / 100, config.fuel_tank_capacity);
    * car = * temp;
    free(temp);

    return true;
}

static int validate_team(char * buffer, race_team_t * team, int * return_flag) {
    char * car_attr, * team_name;
    int team_id;

    if ((car_attr = strtok_r(buffer, DELIM_2, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR, (void *) TEAM, NULL);
        *return_flag = MISSING_ATTR;
        return false;
    } else {
        car_attr = trim_string(car_attr, (int) strlen(car_attr));
        if (strcasecmp(car_attr, TEAM) == 0) {

            if ((team_name = strtok_r(NULL, DELIM_2, &buffer)) == NULL) {
                generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) TEAM, NULL);
                *return_flag = MISSING_ATTR;
                return false;
            }
            team_name = trim_string(team_name, (int) strlen(team_name));

            team_id = get_team_id_by_name(team_name);

            if (team_id == NO_TEAM_SLOT && num_registered_teams == config.num_teams) {
                generate_log_entry(ERROR_TOO_MANY_TEAMS, NULL, NULL);
                *return_flag = NO_TEAM_SLOT;
                return false;
            }

            if (team_id == NEW_TEAM && num_registered_teams < config.num_teams) {
                create_new_team(team_name, &team_id);
                *return_flag = NEW_TEAM;
            }


            * team = shm->race_teams[team_id];

            if (team->num_cars == config.max_cars_per_team) {
                generate_log_entry(ERROR_TOO_MANY_CARS, NULL, NULL);
                *return_flag = NO_CAR_SLOT;
                return false;
            }
            *return_flag = RESULT_NEW_CAR;
            return true;

        } else {
            generate_log_entry(ERROR_INVALID_CAR_ATTR, (void *) car_attr, NULL);
            *return_flag = MISSING_ATTR;
            return false;
        }
    }
}

static int validate_number_attr(char * buffer, const char * expected_attr, float * result) {
    char * cmd_field, * field_value;
    float value;

    if ((cmd_field = strtok_r(buffer, DELIM_2, &buffer)) == NULL) {

        return false;
    }
    cmd_field = trim_string(cmd_field, (int) strlen(cmd_field));

    if (!strcasecmp(cmd_field, expected_attr)) {

        if ((field_value = strtok_r(NULL, DELIM_2, &buffer)) == NULL) {

            return false;
        }

        field_value = trim_string(field_value, (int) strlen(field_value));

        if (to_float(field_value, &value) == FLOAT_CONVERSION_FAILURE || value <= 0) {
            generate_log_entry(ERROR_INVALID_CAR_ATTR_VALUE, (void *) field_value, (void *) expected_attr);

            return false;
        }

        * result = value;

        return true;

    }

    generate_log_entry(ERROR_INVALID_CAR_ATTR, (void *) cmd_field, NULL);

    return false;
}

static int is_car_name_unique(char * car_name) {
    int i, j;
    race_car_t car_B;

    for(i = 0; i < config.num_teams; i++) {
        for(j = 0; j < shm->race_teams[i].num_cars; j++) {
            car_B = shm->race_cars[i][j];
            if (strcasecmp(car_B.name, car_name) == 0) {
                return false;
            }
        }
    }

    return true;
}

int validate_string_attr(char * buffer, const char * expected_attr, char * result) {
    char * cmd_field, * field_value;

    if ((cmd_field = strtok_r(buffer, DELIM_2, &buffer)) == NULL) {

        return false;
    }
    cmd_field = trim_string(cmd_field, (int) strlen(cmd_field));

    if (!strcasecmp(cmd_field, expected_attr)) {

        if ((field_value = strtok_r(NULL, DELIM_2, &buffer)) == NULL) {

            return false;
        }

        field_value = trim_string(field_value, (int) strlen(field_value));

        strcpy(result, field_value);

        return true;

    }

    generate_log_entry(ERROR_INVALID_CAR_ATTR, (void *) cmd_field, NULL);

    return false;
}