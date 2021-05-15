/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include "stdio.h"
#include "string.h"
#include "assert.h"
#include "fcntl.h"
#include "unistd.h"
#include "../../util/process_manager/process_manager.h"
#include "../../util/global.h"
#include "../../util/to_float//to_float.h"
#include "../../util/strings/strings.h"
#include "../../util/file/file.h"
#include "../../ipcs/pipe/pipe.h"
#include "../../race_helpers//log_generator/log_generator.h"
#include "../team_manager/team_manager.h"
#include "race_manager.h"

// endregion dependencies

// region public functions


static void create_teams(int num_teams);
static int interpret_command(char * buffer, race_car_t * car_data);
static int check_start_conditions();
static int validate_car(char * buffer, race_car_t * car);
static int validate_team(char * buffer, race_team_t * team);
static int validate_number(char * buffer, char * expected_cmd, float * result);
static int is_car_id_unique(int car_id, race_team_t * team);
static void handle_named_pipe();
static void handle_all_pipes();
static void notify_race_start();
static void handle_car_state_change(race_car_state_change_t car_state_change, int * end);
static int check_race_end();

int pipe_fds[MAX_NUM_TEAMS + 1];

void race_manager(){

    DEBUG_MSG(PROCESS_RUN, ENTRY, RACE_MANAGER);

    int num_teams = config.num_teams;

    pipe_fds[NAMED_PIPE_INDEX] = open_file(RACE_SIMULATOR_NAMED_PIPE, O_RDONLY | O_NONBLOCK);
    create_teams(num_teams);
    handle_named_pipe();
    notify_race_start(); // TODO: notify team managers instead of cars
    handle_all_pipes();

    wait_procs();

    DEBUG_MSG(PROCESS_EXIT, ENTRY, RACE_MANAGER)
}

void create_teams(int num_teams) {
    int i;
    char team_name[MAX_LABEL_SIZE];
    race_team_t * team = NULL;

    i = 0;

    while (i < num_teams) {
        snprintf(team_name, MAX_LABEL_SIZE * sizeof(char), "%s_%d", TEAM_MANAGER, i);
        team = &shm->race_teams[i];

        create_unn_pipe(unn_pipe_fds);
        pipe_fds[i + 1] = unn_pipe_fds[0];
        strcpy(team->team_name, team_name);
        team->team_id = i;
        create_process(TEAM_MANAGER, team_manager, (void *) team);
        close_fd(unn_pipe_fds[1]);
        i++;
    }
}

void register_car(race_car_t * car) {
    race_team_t * team = car->team;

    shm->race_cars[team->team_id][team->num_cars] = *car;
    shm->race_teams[car->team->team_id].num_cars++;
}

void handle_named_pipe() {
    int n, result, end_read = false;
    char buffer[LARGE_SIZE], aux_buffer[LARGE_SIZE];
    race_car_t car_data;

    while (!end_read) {
        do {
            n = (int) read(pipe_fds[NAMED_PIPE_INDEX], buffer, LARGE_SIZE * sizeof(char));
            if (n > 0) {
                buffer[n - 1]= '\0';
                remove_new_line(buffer, (int) strlen(buffer));
                result = interpret_command(buffer, &car_data);
                strcpy(aux_buffer, buffer);

                switch (result) {
                    case RESULT_NEW_CAR:
                        register_car(&car_data);
                        generate_log_entry(I_CAR_LOADED, (void *) &car_data);
                        break;
                    case RESULT_INVALID_CAR:
                        generate_log_entry(I_CAR_REJECTED, (void *) aux_buffer);
                        break;
                    case RESULT_BEGIN_RACE:
                        end_read = true;
                        break;
                    case RESULT_CANNOT_START_RACE:
                        generate_log_entry(I_CANNOT_START, aux_buffer);
                        break;
                    default:
                        generate_log_entry(I_COMMAND_EXCEPTION, aux_buffer);
                        break;
                }
            }
        } while (n > 0 && !end_read);
    }
}

void handle_all_pipes() {
    fd_set read_set;
    int i, j, k, n;
    char buffer[LARGE_SIZE];
    race_car_state_change_t car_state_change;
    race_box_t * box = NULL;
    race_team_t * team = NULL;
    race_car_t * car = NULL;

    while (true) {
        FD_ZERO(&read_set);

        for (i = 0; i < config.num_teams + 1; i++) {
            FD_SET(pipe_fds[i], &read_set);
        }

        if (select(pipe_fds[config.num_teams] + 1, &read_set, NULL, NULL, NULL) > 0) {
            for (i = 0; i < config.num_teams + 1; i++) {
                if (FD_ISSET(pipe_fds[i], &read_set)) {
                    if (i == NAMED_PIPE_INDEX) {
                        do {
                            n = (int) read(pipe_fds[i], buffer, LARGE_SIZE);

                            if (n > 0) {
                                buffer[n - 1] = '\0';
                                printf("%s\n%s\n", buffer, COMMAND_REJECT);
                            }
                        } while (n > 0);

                        close(pipe_fds[i]);
                        pipe_fds[i] = open_file(RACE_SIMULATOR_NAMED_PIPE, O_RDONLY|O_NONBLOCK);
                    } else {
                        read_stream(pipe_fds[i], (void *) &car_state_change, sizeof(race_car_state_change_t));

                        DEBUG_MSG(CAR_STATE_CHANGE, EVENT, car_state_change.car_id, car_state_change.new_state);

                        switch (car_state_change.new_state) {
                            case RACE:
                                SYNC
                                SYNC_CLOCK_VALLEY
                                shm->num_cars_on_track++;
                                notify_cond(&shm->sync_s.clock_valley_cond);
                                END_SYNC_CLOCK_VALLEY
                                END_SYNC

                                if (car_state_change.prev_state == IN_BOX) {
                                    SYNC
                                    shm->num_refuels++;
                                    END_SYNC
                                }

                                break;
                            case SAFETY:
                                if (car_state_change.malfunctioning) {
                                    SYNC
                                    shm->num_malfunctions++;
                                    END_SYNC
                                }

                                break;

                            case IN_BOX:
                            case DISQUALIFIED:
                                SYNC
                                SYNC_CLOCK_VALLEY
                                shm->num_cars_on_track--;
                                notify_cond(&shm->sync_s.clock_valley_cond);
                                END_SYNC_CLOCK_VALLEY
                                END_SYNC

                                break;
                            case FINISH:
                                SYNC
                                SYNC_CLOCK_VALLEY
                                shm->num_cars_on_track--;
                                notify_cond(&shm->sync_s.clock_valley_cond);
                                END_SYNC_CLOCK_VALLEY
                                END_SYNC

                                if (++shm->num_finished_cars == shm->total_num_cars) {
                                    shm->sync_s.race_running = false;

                                    DEBUG_MSG(CARS_FINISH, EVENT, "")

                                    j = 0;

                                    while (j < config.num_teams) { // notify all the boxes that are waiting for a new car/reservation that the race has finished.
                                        team = &shm->race_teams[j];
                                        box = &team->team_box;

                                        SYNC_BOX_COND
                                        notify_cond_all(&box->cond);
                                        END_SYNC_BOX_COND

                                        while (k < team->num_cars) {
                                            car = &shm->race_cars[j][k];

                                            SYNC_CAR
                                            notify_cond_all(&car->cond);
                                            END_SYNC_CAR

                                            k++;
                                        }

                                        j++;
                                    }

                                    SYNC_CLOCK_VALLEY
                                    notify_cond(&shm->sync_s.clock_valley_cond); // notify the clock that the race is over.
                                    END_SYNC_CLOCK_VALLEY

                                    SYNC_CLOCK_RISE
                                    notify_cond_all(&shm->sync_s.clock_rise_cond); // notify all the threads waiting for the next clock that the race is over.
                                    END_SYNC_CLOCK_RISE

                                    return;
                                }

                                break;
                        }
                    }
                }
            }
        }
    }
}

static void handle_car_state_change(race_car_state_change_t car_state_change, int * end) {
    switch (car_state_change.new_state) {
        case RACE:
            SYNC_CLOCK_VALLEY
            shm->num_cars_on_track++;
            notify_cond(&shm->sync_s.clock_valley_cond);
            END_SYNC_CLOCK_VALLEY

            break;
        case SAFETY:

            break;
        case IN_BOX:
            shm->num_refuels++;

            SYNC_CLOCK_VALLEY
            shm->num_cars_on_track--;
            notify_cond(&shm->sync_s.clock_valley_cond);
            END_SYNC_CLOCK_VALLEY

            if (car_state_change.malfunctioning) {
                shm->num_malfunctions++;
            }

            break;
        case DISQUALIFIED:
            SYNC_CLOCK_VALLEY
            shm->num_cars_on_track--;
            notify_cond(&shm->sync_s.clock_valley_cond);
            END_SYNC_CLOCK_VALLEY


            break;
        case FINISH:
            shm->num_finished_cars++;

            SYNC_CLOCK_VALLEY
            notify_cond(&shm->sync_s.clock_valley_cond);
            END_SYNC_CLOCK_VALLEY

            // TODO: Improve race finish because its still buggy when there are a lot of cars
            if (check_race_end()) {
                * end = true;
            }

            break;
    }
}

static int check_race_end() {
    int i;
    race_box_t * box = NULL;

    if (shm->num_finished_cars == shm->total_num_cars) {
        shm->sync_s.race_running = false;

        i = 0;

        while (i < config.num_teams) { // notify all the boxes that are waiting for a new car/reservation that the race has finished.
            box = &shm->race_teams[i].team_box;
            SYNC_BOX_COND
            notify_cond(&box->cond);
            END_SYNC_BOX_COND

            i++;
        }

        return true;
    }

    return false;
}

void notify_race_start() {
    SYNC
    shm->sync_s.race_running = true;
    notify_cond_all(&shm->sync_s.cond);
    END_SYNC
}

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

// endregion public functions