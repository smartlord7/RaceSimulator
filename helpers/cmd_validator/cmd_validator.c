/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

// region dependencies

#include "malloc.h"
#include "string.h"
#include "assert.h"
#include "../log_generator/log_generator.h"
#include "../../util/global.h"
#include "../../util/strings/strings.h"
#include "../../util/to_float/to_float.h"
#include "../../processes/race_manager/race_manager.h"
#include "cmd_validator.h"

// endregion dependencies

// region private function prototypes

/**
 * @def check_start_conditions
 * @brief Function that checks if the race fills all of the requirements to be started.
 *
 * @return RESULT_CANNOT_START_RACE if the race is ready to start.
 *         RESULT_BEGIN_RACE if the race is not ready to start.
 */
static int check_start_conditions();

/**
 * @def get_team_id_by_name
 * @brief Function that linearly searches for the ID of the team with the given name.
 *
 * @param team_name
 * The name of the team be searched.
 *
 * @return Internal ID of the team with that name (index).
 *         NEW_TEAM if no team has that name.
 *         NO_TEAM_SLOT if no team has that name and there are no more team slots available.
 */
static int get_team_id_by_name(char * team_name);

/**
 * @def validate_car
 * @brief Function that validates the data about a car, after the correspondent CARADD command.
 *
 * @param buffer
 * Buffer with the command data about the car.
 *
 * @param car
 * Car buffer that will store the data if valid.
 *
 * @param team_id
 * The team which owns the car.
 *
 * @return false if the received data is not valid.
 *         true if the received data is valid.
 *
 */
static int validate_car(char * buffer, race_car_t * car, int * team_id);

/**
 * @def validate_team
 * @brief Function that checks if the received team on the command is valid.
 *
 * @param buffer
 * Buffer that holds the data about the team which owns the car.
 *
 * @param team
 * Team buffer that will hold the team data if valid.
 *
 * @param return_flag
 * Flag to allow a further understanding of the result if the team is not valid.
 * Possible values:
 *  - MISSING_ATTR if is missing information about the attribute.
 *  - NO_TEAM_SLOT if it is a new team but no more teams are allowed to be registered.
 *  - NEW_TEAM if it is a new team and can be registered.
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
 * @brief Function that validates a numerical attribute of the car.
 *
 * @param buffer
 * Buffer that holds the numerical attribute.
 *
 * @param expected_attr
 * Expected attribute name.
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
 * @brief Function that verifies if the car name is unique amogns all the race cars.
 *
 * @param car_name
 * The car name to be searched.
 *
 * @return false if a car already has that name.
 *         true if no other car has that name.
 */
static int is_car_name_unique(char * car_name);

/**
 * @def validate_string_attr
 * @brief Function that validates a string attribute of the car.
 *
 * @param buffer
 * Buffer that holds the string attribute.
 *
 * @param expected_attr
 * Expected attribute name.
 *
 * @param result
 * Value of the string attribute.
 *
 * @return false if the attribute is not valid.
 *         true if the attribute is valid.
 */
static int validate_string_attr(char * buffer, const char * expected_attr, char * result);

// endregion private function prototypes

// region global variables

int num_registered_teams;

// endregion global variables

// region public functions

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

// endregion public functions

// region private functions

static int check_start_conditions() {
    if (num_registered_teams == config.num_teams) {
        return RESULT_BEGIN_RACE;
    }

    generate_log_entry(ERROR_NOT_ENOUGH_TEAMS, NULL, NULL);

    return RESULT_CANNOT_START_RACE;
}


static int validate_car(char * buffer, race_car_t * car, int * team_id) {
    assert(buffer != NULL && car != NULL && team_id != NULL);

    float speed, consumption, reliability;
    char * token = NULL, car_name[LARGE_SIZE], aux[LARGEST_SIZE];
    int return_flag;
    race_team_t team = {0};
    race_car_t * temp = NULL;

    // validate team name.
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

    // validate car name.
    if ((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) CAR, NULL);
        return false;
    }
    strcpy(aux, token);
    if (!validate_string_attr(aux, CAR, car_name))  {
        return false;
    }
    // check if any car already has that name.
    if (!is_car_name_unique(car_name)) {
        generate_log_entry(ERROR_UNIQUE_CONSTRAINT_VIOLATED, (void *) CAR, car_name);
        return false;
    }

    // validate speed.
    if ((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) SPEED, NULL);
        return false;
    }
    strcpy(aux, token);
    if (!validate_number_attr(aux, SPEED, &speed) || speed <= 0) {
        return false;
    }

    // validate consumption.
    if ((token = strtok_r(NULL, DELIM_1, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) CONSUMPTION, NULL);
        return false;
    }
    strcpy(aux, token);
    if (!validate_number_attr(aux, CONSUMPTION, &consumption) || consumption <= 0)  {
        return false;
    }

    // validate reliability.
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
    assert(buffer != NULL && team != NULL && return_flag != NULL);

    char * car_attr = NULL, * team_name = NULL;
    int team_id;

    //check if the buffer contents correspond to what is expected.
    if ((car_attr = strtok_r(buffer, DELIM_2, &buffer)) == NULL) {
        generate_log_entry(ERROR_MISSING_CAR_ATTR, (void *) TEAM, NULL);
        * return_flag = MISSING_ATTR;

        return false;
    } else {
        car_attr = trim_string(car_attr);
        //check if the command field is what is expected.
        if (strcasecmp(car_attr, TEAM) == 0) {

            // obtain the team name.
            if ((team_name = strtok_r(NULL, DELIM_2, &buffer)) == NULL) {
                generate_log_entry(ERROR_MISSING_CAR_ATTR_VALUE, (void *) TEAM, NULL);
                * return_flag = MISSING_ATTR;

                return false;
            }
            team_name = trim_string(team_name);

            // check if there is an existing team with that name.
            team_id = get_team_id_by_name(team_name);

            // no team has that name and all of team slots are filled
            if (team_id == NO_TEAM_SLOT && num_registered_teams == config.num_teams) {
                generate_log_entry(ERROR_TOO_MANY_TEAMS, NULL, NULL);
                *return_flag = NO_TEAM_SLOT;

                return false;
            }

            // no team has that name and therefore a new team needs to be created.
            if (team_id == NEW_TEAM && num_registered_teams < config.num_teams) {
                create_new_team(team_name, &team_id);
                * return_flag = NEW_TEAM;
            }

            * team = shm->race_teams[team_id];

            // check if the team can register one more car.
            if (team->num_cars == config.max_cars_per_team) {
                generate_log_entry(ERROR_TOO_MANY_CARS, NULL, NULL);
                * return_flag = NO_CAR_SLOT;

                return false;
            }
            * return_flag = RESULT_NEW_CAR;

            return true;

        } else {
            generate_log_entry(ERROR_INVALID_CAR_ATTR, (void *) car_attr, NULL);
            * return_flag = MISSING_ATTR;

            return false;
        }
    }
}

static int validate_number_attr(char * buffer, const char * expected_attr, float * result) {
    assert(buffer != NULL && expected_attr != NULL && result != NULL);

    char * cmd_field = NULL, * field_value = NULL;
    float value;

    //obtain the command field of the segment.
    if ((cmd_field = strtok_r(buffer, DELIM_2, &buffer)) == NULL) {

        return false;
    }
    cmd_field = trim_string(cmd_field);

    //check if it is what is expected.
    if (!strcasecmp(cmd_field, expected_attr)) {

        if ((field_value = strtok_r(NULL, DELIM_2, &buffer)) == NULL) {

            return false;
        }

        field_value = trim_string(field_value);

        // convert the numerical value to a float.
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
    assert(car_name != NULL);

    int i, j;
    race_car_t car_B = {0};

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

static int validate_string_attr(char * buffer, const char * expected_attr, char * result) {
    assert(buffer != NULL && expected_attr != NULL && result != NULL);

    char * cmd_field = NULL, * field_value = NULL;

    //obtain the command field
    if ((cmd_field = strtok_r(buffer, DELIM_2, &buffer)) == NULL) {

        return false;
    }
    cmd_field = trim_string(cmd_field);

    //check if it is what is expected
    if (!strcasecmp(cmd_field, expected_attr)) {

        if ((field_value = strtok_r(NULL, DELIM_2, &buffer)) == NULL) {

            return false;
        }

        field_value = trim_string(field_value);

        strcpy(result, field_value);

        return true;

    }

    generate_log_entry(ERROR_INVALID_CAR_ATTR, (void *) cmd_field, NULL);

    return false;
}

static int get_team_id_by_name(char * team_name) {
    assert(team_name != NULL);

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

// endregion private functions
