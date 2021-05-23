/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

// region dependencies

#include <malloc.h>
#include "string.h"
#include "assert.h"
#include "cmd_validator.h"
#include "../../util/global.h"
#include "../../util/strings/strings.h"
#include "../../util/to_float/to_float.h"
#include "../../processes/race_manager/race_manager.h"
#include "../log_generator/log_generator.h"

// endregion dependencies

// region private function prototypes

/**
 * @def check_start_conditions
 * @brief Function that checks if the race fills all of the requirements to be started.
 *
 * @return RESULT_CANNOT_START_RACE if the race does not fill all of the requirements to be started.
 *         RESULT_BEGIN_RACE if the race fills all of the requirements to be started.
 */
static int check_start_conditions();

/**
 * @def get_team_id_by_name
 * @brief Function that retrieves the ID of the team with the given name.
 *
 * @param team_name
 * Name to be searched.
 *
 * @return Internal ID of the team with that name.
 *         NEW_TEAM if no team has that name.
 *         NO_TEAM_SLOT if no team has that name and there are no more team slots available.
 */
static int get_team_id_by_name(char * team_name);

/**
 * @def validate_car
 * @brief Function that validates the data about a car on a given command.
 *
 * @param buffer
 * Command with the data about the car.
 *
 * @param car
 * Struct to store the data if it is valid.
 *
 * @param team_id
 * Team ID of team who owns the car and the data is valid.
 *
 * @return false if the received data is not valid.
 *         true if the received data is valid.
 *
 */
static int validate_car(char * buffer, race_car_t * car, int * team_id);

/**
 * @def validate_team
 * @brief Function that checks if the received the team on the command is valid.
 *
 * @param buffer
 * Command segment with data about the team who owns the car.
 *
 * @param team
 * Struct to store the team data if valid.
 *
 * @param return_flag
 * Flag to allow a further understanding of the result if the team is not valid.
 * Possible values:
 *  - MISSING_ATTR if is missing information about the attribute.
 *  - NO_TEAM_SLOT if it is a new team but no more teams are allowed to be registered.
 *  - NEW_TEAM if it is a new team.
 *  - NO_CAR_SLOT if the team cannot register any more cars.
 *  - RESULT_NEW_CAR if the car can be registered by the team.
 *
 * @return false if the team is not valid.
 *         true if the team is valid.
 *
 */
static int validate_team(char * buffer, race_team_t * team, int * return_flag);

/**
 * @def validate_number_attr
 * @brief Function that validates a numerical attribute.
 *
 * @param buffer
 * Segment with the numerical attribute.
 *
 * @param expected_attr
 * Expected meaning for the segment.
 *
 * @param result
 * Float value of the converted numerical attribute.
 *
 * @return false if the numerical attribute is not valid.
 *         true if the numerical attribute is valid.
 */
static int validate_number_attr(char * buffer, const char * expected_attr, float * result);

/**
 * @def is_car_name_unique
 * @brief Function that verifies if car name is unique.
 *
 * @param car_name
 * Name to be compared.
 *
 * @return false if a car already has that name.
 *         true if no other car already has that name.
 */
static int is_car_name_unique(char * car_name);

/**
 * @def validate_string_attr
 * @brief Function that validates a string attribute.
 *
 * @param buffer
 * Segment with the string attribute.
 *
 * @param expected_attr
 * Expected meaning for the segment.
 *
 * @param result
 * Value of the string attribute.
 *
 * @return false if the attribute is not valid.
 *         true if the attribute is valid.
 */
static int validate_string_attr(char * buffer, const char * expected_attr, char * result);

// endregion private function prototypes

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
    } else if (strcasecmp(buffer, HELP_REQ) == 0) {
        return RESULT_HELP;
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
        //a team already has that name
        if (strcasecmp(shm->race_teams[i].team_name, team_name) == 0) {
            return shm->race_teams[i].team_id;
        }
        //no team has that name therefore it is to create a new one
        if (shm->race_teams[i].team_id < 0) {
            return NEW_TEAM;
        }
    }

    //no team has such name and all the slots for the teams are filled
    return NO_TEAM_SLOT;
}

static int validate_car(char * buffer, race_car_t * car, int * team_id) {
    assert(buffer != NULL);

    float speed, consumption, reliability;
    char * token = NULL, car_name[LARGE_SIZE], aux[LARGEST_SIZE];
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
    //check if any car already has that name
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

    //check if the buffer contents correspond to what is expected
    if ((car_attr = strtok_r(buffer, DELIM_2, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR, (void *) TEAM, NULL);
        *return_flag = MISSING_ATTR;
        return false;
    } else {
        car_attr = trim_string(car_attr, (int) strlen(car_attr));
        //check if the command field is what is expected
        if (strcasecmp(car_attr, TEAM) == 0) {

            //obtain the team name
            if ((team_name = strtok_r(NULL, DELIM_2, &buffer)) == NULL) {
                generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) TEAM, NULL);
                *return_flag = MISSING_ATTR;
                return false;
            }
            team_name = trim_string(team_name, (int) strlen(team_name));

            //check if a team already has that name
            team_id = get_team_id_by_name(team_name);

            //no team has that name and all of team slots are filled
            if (team_id == NO_TEAM_SLOT && num_registered_teams == config.num_teams) {
                generate_log_entry(ERROR_TOO_MANY_TEAMS, NULL, NULL);
                *return_flag = NO_TEAM_SLOT;
                return false;
            }

            //no team has that name and therefore is it to create a new team
            if (team_id == NEW_TEAM && num_registered_teams < config.num_teams) {
                create_new_team(team_name, &team_id);
                *return_flag = NEW_TEAM;
            }

            * team = shm->race_teams[team_id];

            //check if the team can register one more car
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

    //obtain the command field of the segment
    if ((cmd_field = strtok_r(buffer, DELIM_2, &buffer)) == NULL) {

        return false;
    }
    cmd_field = trim_string(cmd_field, (int) strlen(cmd_field));

    //check if it is what is expected
    if (!strcasecmp(cmd_field, expected_attr)) {

        if ((field_value = strtok_r(NULL, DELIM_2, &buffer)) == NULL) {

            return false;
        }

        field_value = trim_string(field_value, (int) strlen(field_value));

        // convert the numerical value to a float
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

    //obtain the command field
    if ((cmd_field = strtok_r(buffer, DELIM_2, &buffer)) == NULL) {

        return false;
    }
    cmd_field = trim_string(cmd_field, (int) strlen(cmd_field));

    //check if it is what is expected
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