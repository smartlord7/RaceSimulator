/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include <ctype.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "fcntl.h"
#include "sys/unistd.h"
#include "../../util/process_manager/process_manager.h"
#include "../../util/global.h"
#include "../../util/to_float//to_float.h"
#include "../../util/strings/strings.h"
#include "../../util/file/file.h"
#include "../../ipcs/pipe/pipe.h"
#include "../../util/log_generator/log_generator.h"
#include "../team_manager/team_manager.h"
#include "race_manager.h"

// endregion dependencies

// region public functions

int read_command(int fd, char * buffer, int size);

void create_teams(int num_teams, int pipes_read_fds[MAX_NUM_TEAMS]);

int interpret_command(char * buffer, race_car_t * car_data);

int check_start_conditions();

int validate_add(char * buffer, race_car_t * read_data);

int validate_team(char * buffer, race_team_t * team);

int validate_car_id(char * buffer, int * id, race_team_t * team);

int validate_speed(char * buffer, float * speed);

int validate_consumption(char * buffer, float * consumption);

int validate_reliability(char * buffer, float * reliability);

void handle_named_pipe(int fd);

void handle_all_pipes(const int pipes_read_fds[MAX_NUM_TEAMS]);

void notify_race_start();

void race_manager(){

    DEBUG_MSG(PROCESS_RUN, ENTRY, RACE_MANAGER);

    int num_teams = config.num_teams, pipes_read_fds[MAX_NUM_TEAMS];
    shm->total_num_cars = 0;

    pipes_read_fds[NAMED_PIPE_INDEX] = open_file(RACE_SIMULATOR_NAMED_PIPE, O_RDONLY | O_NONBLOCK);
    handle_named_pipe(pipes_read_fds[NAMED_PIPE_INDEX]);
    create_teams(num_teams, pipes_read_fds);
    notify_race_start();
    handle_all_pipes(pipes_read_fds);

    wait_procs();

    DEBUG_MSG(PROCESS_EXIT, ENTRY, RACE_MANAGER)
}

void create_teams(int num_teams, int fds[MAX_NUM_TEAMS]) {
    int i;
    race_team_t * team = NULL;

    i = 0;

    while (i < num_teams) {
        char team_name[MAX_LABEL_SIZE];
        snprintf(team_name, MAX_LABEL_SIZE * sizeof(char), "%s_%d", TEAM_MANAGER, i);
        team = &shm->race_teams[i];

        create_unn_pipe(unn_pipe_fds);
        close_fd(unn_pipe_fds[1]);
        fds[i] = unn_pipe_fds[0];

        strcpy(team->team_name, team_name);
        team->team_id = i;
        create_process(TEAM_MANAGER, team_manager, (void *) team);
        i++;
    }
}

void register_car(race_car_t car) {
    race_team_t * team = car.team;

    SYNC;
    shm->race_teams[car.team->team_id].num_cars++;
    shm->race_cars[team->team_id][team->num_cars] = car;
    END_SYNC;

}

void handle_named_pipe(int fd) {
    int n, result, end_read = false;
    char buffer[LARGE_SIZE];
    race_car_t car_data;

    while(!end_read) {
        do {
            n = (int) read(fd, buffer, LARGE_SIZE * sizeof(char));
            if (n > 0) {
                buffer[n - 1] = '\0';

                interpret_command(buffer, &car_data);

                switch (result) {
                    case RESULT_NEW_CAR:
                        register_car(car_data);
                        break;
                    case RESULT_REJECTED_CAR:
                        generate_log_entry(I_CAR_REJECTED, (void *) buffer);
                        break;
                    case RESULT_BEGIN_RACE:
                        end_read = true;
                        break;
                    case RESULT_CANNOT_START_RACE:
                        generate_log_entry(I_CANNOT_START, NULL);
                        break;
                    default:

                        break;
                }
            }
        } while(n > 0 && !end_read);
    }
}

void handle_all_pipes(const int pipes_read_fds[MAX_NUM_TEAMS]) {
    fd_set read_set;
    int i, n;
    char buffer[LARGE_SIZE];
    race_car_state_change_t car_state_change;

    while (true) {
        FD_ZERO(&read_set);

        for (i = 0; i < config.num_teams + 1; i++) {
            FD_SET(pipes_read_fds[i], &read_set);
        }

        if (select(pipes_read_fds[config.num_teams] + 1, &read_set, NULL, NULL, NULL) > 0) {
            for (i = 0; i < config.num_teams + 1; i++) {
                if (FD_ISSET(pipes_read_fds[i], &read_set)) {
                    if (i == NAMED_PIPE_INDEX) {
                        do {
                            n = (int) read(pipes_read_fds[i], buffer, LARGE_SIZE);

                            if (n > 0) {
                                buffer[n - 1] = '\0';
                                printf("%s\n%s\n", buffer, COMMAND_REJECT);
                            }
                        } while (n > 0);
                    } else {
                        read_stream(pipes_read_fds[i], (void *) &car_state_change, sizeof(struct race_car_state_change_t));

                        DEBUG_MSG(CAR_STATE_CHANGE, COMMS, car_state_change.car_id, car_state_change.new_state);
                    }
                }
            }
        }
    }
}

void notify_race_start() {
    int i, j;
    race_car_t * current_car;

    for (i = 0; i < config.num_teams; i++) {
        for (j = 0; j < shm->race_teams[i].num_cars; j++) {
            current_car = &shm->race_cars[i][j];

            lock_mutex(&current_car->cond_mutex);
            shm->sync_s.race_start = true;
            notify_all_cond(&current_car->cond);
            unlock_mutex(&current_car->cond_mutex);
        }
    }
    lock_mutex(&shm->sync_s.mutex);
    shm->sync_s.race_start = true;
    notify_all_cond(&shm->sync_s.cond);
    unlock_mutex(&shm->sync_s.mutex);
}

int interpret_command(char * buffer, race_car_t * car_data) {
    char command_preview[COMMAND_PREVIEW_SIZE];

    //used to take a hint on what the command might be
    memcpy(command_preview, buffer, sizeof(command_preview) - sizeof(char));
    command_preview[COMMAND_PREVIEW_SIZE - 1] = '\0';

    printf("preview: %s\n", command_preview);

    //start to filter possibilities
    if(strcasecmp(command_preview, START) == 0) {

        if(strcasecmp(buffer, START_RACE) == 0) return check_start_conditions();

    } else if(strcasecmp(command_preview, ADDCAR) == 0) {
        buffer = buffer + COMMAND_PREVIEW_SIZE;

        HERE("1");

        if(validate_add(buffer, car_data) == true) return RESULT_NEW_CAR;
        return RESULT_REJECTED_CAR;
    }

    return RESULT_INVALID_COMMAND;
}

int check_start_conditions() {
    int i;

    for(i = 0; i < MAX_NUM_TEAMS; i++) {
        if(shm->race_teams[i].num_cars == 0) return RESULT_CANNOT_START_RACE;
    }

    return RESULT_BEGIN_RACE;
}

race_team_t * get_team(char * team_name) {
    int i;

    for(i = 0; i < config.num_teams && shm->race_teams[i].team_id > 0; i++) {

        if(strcasecmp(shm->race_teams[i].team_name, team_name) == 0) {

            if(shm->race_teams[i].num_cars == config.max_cars_per_team) {
                return NULL;
            }
            return &shm->race_teams[i];
        }
    }

    return NULL;
}

int validate_add(char * buffer, race_car_t * car) {
    char * token;
    race_team_t * team;
    int car_id;
    float speed, consumption, reliability;
    HERE("2");

    // validate team name
    if((token = strtok(buffer, DELIM_1)) == NULL) {
        return false;
    }
    if(validate_team(token, team) == false) return false;


    //validate car number
    if((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if(validate_car_id(token, &car_id, team) == false) return false;

    HERE("4");

    //validate speed
    if((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if(validate_speed(token, &speed) == false) return false;

    // validate consumption
    if((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if(validate_consumption(token, &consumption) == false) return false;

    // validate reliability
    if((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if(validate_reliability(token, &reliability) == false) return false;

    car = race_car(team, car_id, consumption, speed, reliability, config.fuel_tank_capacity);
    if(car == NULL) return false;
    return true;
}

int validate_team(char * buffer, race_team_t * team) {
    char *team_field, *team_name;

    if ((team_field = strtok(buffer, ":")) == NULL) return false;
    else {
        team_field = trim_string(team_field, strlen(team_field));
        if (strcasecmp(team_field, TEAM) == 0) {

            if ((team_name = strtok(NULL, DELIM_2)) == NULL) return false;
            team_name = trim_string(team_name, strlen(team_name));

            team = get_team(team_name);
            if (team == NULL) return false;

            return true;

        } else return false;
    }
}

int validate_int(char * buffer, char * expected_cmd, int * result) {
    char * cmd_field, * value_field;
    int value;

    if((cmd_field = strtok(buffer, ":")) == NULL) return false;
    else {

        if(!strcasecmp(cmd_field, expected_cmd)) {

            if((value_field = strtok(NULL, DELIM_2)) == NULL) return false;
            value_field = trim_string(value_field, strlen(value_field));

            if(!(value = atoi(value_field)) || value <= 0) return false;
            *result = value;

            return true;

        } else return false;

    }
}

int validate_float(char * buffer, char * expected_cmd, float * result) {
    char * cmd_field, * value_field;
    float value;

    if((cmd_field = strtok(buffer, ":")) == NULL) return false;
    else {

        if(!strcasecmp(cmd_field, expected_cmd)) {

            if((value_field = strtok(NULL, DELIM_2)) == NULL) return false;

            value_field = trim_string(value_field, strlen(value_field));

            if(to_float(value_field, &value) == FLOAT_CONVERSION_FAILURE || value <= 0) return false;

            *result = value;

            return true;

        } else return false;

    }
}

int check_unique_id(int car_id, race_team_t * team) {
    int i, j;
    race_car_t car_B;

    for(i = 0; i < MAX_NUM_TEAMS; i++) {
        for(j = 0; j < MAX_MAX_CARS_PER_TEAM; j++) {
            car_B = shm->race_cars[i][j];
            if(car_B.team->team_id == team->team_id && car_B.car_id == car_id) return false;
        }

    }

    return true;
}

int validate_car_id(char * buffer, int * id, race_team_t * team) {

    if(validate_int(buffer, CAR, id)) {
        return *id >= 0 && check_unique_id(*id, team);
    }
}

int validate_speed(char * buffer, float * speed) {

    return validate_float(buffer, SPEED, speed) && *speed > 0;

}

int validate_consumption(char * buffer, float * consumption) {

    return validate_float(buffer, CONSUMPTION, consumption) && *consumption > 0;
}

int validate_reliability(char * buffer, float * reliability) {

    return validate_int(buffer, RELIABILITY, reliability) && *reliability > 0;
}

// endregion public functions