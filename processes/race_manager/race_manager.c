/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

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
#include "../team_manager/team_manager.h"
#include "race_manager.h"

// endregion dependencies

// region public functions

int read_command(int fd, char * buffer, int size);

void create_teams(int num_teams, int pipes_read_fds[MAX_NUM_TEAMS]);

int interpret_command(char * buffer, race_car_t * car_data);

int check_start_conditions();

int validate_add(char * buffer, race_car_t * read_data);

int validate_team(char * buffer, race_car_t * read_data);

int validate_car_id(char * buffer, race_car_t  * read_data);

int validate_speed(char * buffer, race_car_t  * read_data);

int validate_consumption(char * buffer, race_car_t  * read_data);

int validate_reliability(char * buffer, race_car_t  * read_data);

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

void handle_named_pipe(int fd) {
    int n, end_read = false;
    char buffer[LARGE_SIZE];

    while(!end_read) {
        do {
            n = (int) read(fd, buffer, LARGE_SIZE * sizeof(char));
            if (n > 0) {
                buffer[n - 1] = '\0';
                // TODO validate commands

                if (strcmp(buffer, START_RACE) == 0) {
                    end_read = true;
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


int read_command(int fd, char * buffer, int size){
    int n;

    do {
        n = read(fd, buffer, size);
    } while(n > 0);

    printf("buffer: %s\n", buffer);

    if(n < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
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

        if(validate_add(buffer, car_data)) return RESULT_NEW_CAR;

    }

    return RESULT_INVALID_COMMAND;
}

int check_start_conditions() {

    return 1;
}

race_team_t * get_team(char * team_name) {
    int i;

    for(i = 0; i < config.num_teams; i++) {
        if(strcasecmp(shm->race_teams[i].team_name, team_name) == 0) return &shm->race_teams[i];
    }

    return NULL;
}

int validate_add(char * buffer, race_car_t * read_data) {
    char * token;
    HERE("2");

    // validate team name
    if((token = strtok(buffer, DELIM_1)) == NULL) {
        return false;
    }
    HERE("3");
    if(validate_team(token, read_data) == false) return false;


    //validate car number
    if((token = strtok(NULL, DELIM_1)) == NULL) {
        return false;
    }
    if(validate_car_id(token, read_data) == false) return false;

    HERE("4");

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

    HERE("5");
    return true;
}

int validate_team(char * buffer, race_car_t * read_data) {
    char * addcar_team, * team_name;
    race_team_t * team;

    if((addcar_team = strtok(buffer, ":")) == NULL) return false;
    else {
        HERE("8");
        addcar_team = trim_string(addcar_team, strlen(addcar_team));
        printf("%s-%lu\n%s-%lu\n", addcar_team, strlen(addcar_team), TEAM, strlen(TEAM));
        if(!strcasecmp(addcar_team, TEAM)) {

            HERE("9");

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
    race_car_t * car_B;

    for(i = 0; i < MAX_NUM_TEAMS && shm->race_cars[i] != NULL; i++) {

        car_B = shm->race_cars[i];
        if(car_B->team->team_id == car_data->team->team_id && car_B->car_id == car_id) return false;

    }

    car_data->car_id = car_id;

    return true;
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