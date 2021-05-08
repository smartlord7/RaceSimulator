/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include "../../util/process_manager/process_manager.h"
#include "../../util/global.h"
#include "../team_manager/team_manager.h"
#include "race_manager.h"
#include "../../util/to_float//to_float.h"
#include "../../util/strings/strings.h"

// endregion dependencies

// region public functions

int read_command(int fd, char * buffer, int size);

void create_teams(int num_teams);

int interpret_command(char * buffer, race_car_t * car_data);

int check_start_conditions();

int validate_add(char * buffer, race_car_t * read_data);

int validate_team(char * buffer, race_car_t * read_data);

int validate_car_id(char * buffer, race_car_t  * read_data);

int validate_speed(char * buffer, race_car_t  * read_data);

int validate_consumption(char * buffer, race_car_t  * read_data);

int validate_reliability(char * buffer, race_car_t  * read_data);

void watch_pipe(void * fd);

int n_pipe_fd;
pthread_t watcher_id;

void race_manager(){

    DEBUG_MSG(PROCESS_RUN, DEBUG_LEVEL_ENTRY, RACE_MANAGER);

    int num_teams = config.num_teams;

    //create the teams' processes
    create_teams(num_teams);

    n_pipe_fd = fd_named_pipe;

    // cria thread watch pipe
    watcher_id = (pthread_t) 1;
    //create_thread(N_PIPE_WATCHER, &watcher_id, (void *) watch_pipe, n_pipe_fd);

    //register

    //wait for all the child processes
    wait_procs();

    DEBUG_MSG(PROCESS_EXIT, DEBUG_LEVEL_ENTRY, RACE_MANAGER)

    exit(EXIT_SUCCESS);
}

void create_teams(int num_teams) {
    int i;
    race_team_t * team = NULL;

    while (i < num_teams) {
        char team_name[MAX_LABEL_SIZE];
        snprintf(team_name, MAX_LABEL_SIZE * sizeof(char), "%s_%d", TEAM_MANAGER, i);
        team = &shm->race_teams[i];

        strcpy(team->team_name, team_name);
        team->team_id = i;
        create_process(TEAM_MANAGER, team_manager, (void *) team);
        i++;
    }
}

void watch_pipe(void * fd) {
    int result_type;
    char buffer[MAX_BUFFER_SIZE];
    race_team_t * team = NULL;
    race_car_t * car = NULL;

    while(true) {

        if(read_command(n_pipe_fd, buffer, sizeof(buffer))) {

            result_type = interpret_command(buffer, car);

            switch (result_type) {

            }

        }
    }
}

int read_command(int fd, char * buffer, int size){
    int n;

    do {
        n = read(fd, buffer, size);
    } while(n > 0);

    if(n < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int interpret_command(char * buffer, race_car_t * car_data) {
    char command_preview[COMMAND_PREVIEW_SIZE];

    //used to take a hint on what the command might be
    memcpy(command_preview, buffer, sizeof(command_preview) - sizeof(char));
    command_preview[COMMAND_PREVIEW_SIZE - 1] = '\0';

    //start to filter possibilities
    if(strcasecmp(command_preview, START) == 0) {

        if(strcasecmp(buffer, START_RACE) == 0) return check_start_conditions();

    } else if(strcasecmp(command_preview, ADDCAR) == 0) {
        buffer = buffer + COMMAND_PREVIEW_SIZE;

        if(validate_add(buffer, car_data)) return RESULT_NEW_CAR;

    }

    return RESULT_INVALID_COMMAND;
}

int check_start_conditions() {

}

race_team_t * get_team(char * team_name) {
    int i;

    for(i = 0; i < config.num_teams; i++) {
        if(strcasecmp(shm->race_teams[i].team_name, team_name) == 0) return &shm->race_teams[i];
    }

    return NULL;
}

int validate_add(char * buffer, race_car_t * read_data) {
    int result;
    char * token, * tokenB;

    char * speed, * speed_value;
    char * consumption, * consumption_value;
    char * reliability, * reliabilty_value;

    // validate team name
    if((token = strtok(buffer, DELIM_1)) == NULL) {
        return false;
    }
    if(validate_team(token, read_data) == false) return false;

    //validate car number
    if((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    result = validate_car_id(token, read_data);
    if(result == false) return result;

    //validate speed
    if((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if(validate_speed(token, read_data) == false) return false;

    // validate consumption
    if((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if(validate_consumption(token, read_data) == false) return false;

    // validate reliability
    if((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if(validate_reliability(token, read_data) == false) return false;

    return true;
}

int validate_team(char * buffer, race_car_t * read_data) {
    char * addcar_team, * team_name;
    race_team_t * team;

    if((addcar_team = strtok(buffer, ":")) == NULL) return false;
    else {

        if(strcasecmp(addcar_team, TEAM)) {

            if((team_name = strtok(NULL, DELIM_2)) == NULL) return false;
            team_name = trim_string(team_name, strlen(team_name));

            team = get_team(team_name);
            if(team == NULL) return false;

            read_data->team = team;
            return true;

        } else return false;
    }
}

int check_unique_id(int car_id, race_car_t * car_data) {
    int i;
    race_car_t * car_A = car_data, *car_B;

    for(i = 0; i < MAX_NUM_TEAMS && shm->race_cars[i] != NULL; i++) {

        car_B = shm->race_cars[i];
        if(car_B->team->team_id == car_A->team->team_id && car_B->car_id == car_A->car_id) return false;

    }

    return true;
}

int validate_int(char * buffer, char * expected_cmd, int * result) {
    char * cmd_field, * value_field;
    int value;

    if((cmd_field = strtok(buffer, ":")) == NULL) return false;
    else {

        if(strcasecmp(cmd_field, expected_cmd)) {

            if((value_field = strtok(NULL, DELIM_2)) == NULL) return false;
            value_field = trim_string(value_field, strlen(value_field));

            if(!(value = atoi(value_field)) || value <= 0) return false;

            return true;

        } else return false;

    }
}

int validate_float(char * buffer, char * expected_cmd, float * result) {
    char * cmd_field, * value_field;
    float value;

    if((cmd_field = strtok(buffer, ":")) == NULL) return false;
    else {

        if(strcasecmp(cmd_field, expected_cmd)) {

            if((value_field = strtok(NULL, DELIM_2)) == NULL) return false;

            value_field = trim_string(value_field, strlen(value_field));

            if(to_float(value_field, &value) == FLOAT_CONVERSION_FAILURE || value <= 0) return false;

            *result = value;

            return true;

        } else return false;

    }
}

int validate_car_id(char * buffer, race_car_t  * car_data) {
    int car_id;

    if(validate_int(buffer, CAR, &car_id)) {

        if(!check_unique_id(car_id, car_data)) return false;

        car_data->car_id = car_id;

        return true;
    }

    return false;
}

int validate_speed(char * buffer, race_car_t  * car_data) {
    float speed;

    if(validate_float(buffer, SPEED, &speed)) {
        car_data->speed = speed;

        return true;
    }

    return false;
}

int validate_consumption(char * buffer, race_car_t  * car_data) {
    float consumption;

    if(validate_float(buffer, CONSUMPTION, &consumption)) {
        car_data->consumption = consumption;

        return true;
    }

    return false;
}

int validate_reliability(char * buffer, race_car_t  * car_data) {
    int reliability;

    if (validate_int(buffer, RELIABILITY, &reliability)) {

        if (!check_unique_id(reliability, car_data)) return false;

        car_data->car_id = reliability;

        return true;
    }

    return false;
}

// endregion public functions