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
#include "fcntl.h"
#include "unistd.h"
#include "../../util/process_manager/process_manager.h"
#include "../../util/global.h"
#include "../../util/file/file.h"
#include "../../ipcs/pipe/pipe.h"
#include "../../race_helpers//log_generator/log_generator.h"
#include "../team_manager/team_manager.h"
#include "race_manager.h"
#include "../../race_helpers/cmd_validator/cmd_validator.h"
#include "../../race_helpers/stats_helper/stats_helper.h"

// endregion dependencies

// region public functions

static void initialize_team_slots(int num_teams);
static void handle_named_pipe();
static void handle_all_pipes();
static void notify_race_start();
static void register_car(race_car_t * car, int team_id);
static int check_race_end();

int pipe_fds[MAX_NUM_TEAMS + 1];

void race_manager(){

    DEBUG_MSG(PROCESS_RUN, ENTRY, RACE_MANAGER);

    int num_teams = config.num_teams;
    num_registed_teams = 0;

    pipe_fds[NAMED_PIPE_INDEX] = open_file(RACE_SIMULATOR_NAMED_PIPE, O_RDONLY | O_NONBLOCK);
    initialize_team_slots(num_teams);
    handle_named_pipe();
    notify_race_start();
    handle_all_pipes();

    wait_procs();

    DEBUG_MSG(PROCESS_EXIT, ENTRY, RACE_MANAGER)
}

void handle_named_pipe() {
    int n, end_read = false;
    char buffer[LARGEST_SIZE * 10], buffer2[LARGEST_SIZE * 10], * aux, * aux2;
    race_car_t car_data;
    int result, team_id;

    while (!end_read) {
        do {
            n = (int) read(pipe_fds[NAMED_PIPE_INDEX], buffer, LARGEST_SIZE * 10 * sizeof(char));

            if (n > 0) {
                buffer[n - 1]= '\0';
                aux = strtok_r(buffer, DELIM_3, &aux2);

                do {
                    strcpy(buffer2, aux);
                    result = interpret_command(aux, &car_data, &team_id);

                    switch (result) {
                        case RESULT_NEW_CAR:
                            register_car(&car_data, team_id);
                            generate_log_entry(CAR_LOAD, (void *) &car_data, NULL);
                            break;
                        case RESULT_INVALID_CAR:
                            generate_log_entry(CAR_REJECT, buffer2 + strlen(ADDCAR) + 1, NULL);
                            break;
                        case RESULT_BEGIN_RACE:
                            generate_log_entry(RACE_START, NULL, NULL);
                            end_read = true;
                            break;
                        case RESULT_CANNOT_START_RACE:
                            generate_log_entry(RACE_CANNOT_START, buffer2, NULL);
                            break;
                        default:
                            generate_log_entry(COMMAND_REJECT, buffer2, NULL);
                            break;
                    }
                } while ((aux = strtok_r(NULL, DELIM_3, &aux2)) != NULL);
            }
        } while (n > 0 && !end_read);
    }
}

void handle_all_pipes() {
    fd_set read_set;
    int i, n, race_winner = false;
    char buffer[LARGE_SIZE];
    race_car_state_change_t car_state_change;
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
                                generate_log_entry(COMMAND_REJECT2, (void *) buffer, NULL);
                            }
                        } while (n > 0);

                        close(pipe_fds[i]);
                        pipe_fds[i] = open_file(RACE_SIMULATOR_NAMED_PIPE, O_RDONLY|O_NONBLOCK);
                    } else {
                        read_stream(pipe_fds[i], (void *) &car_state_change, sizeof(race_car_state_change_t));
                        car = &shm->race_cars[car_state_change.team_id][car_state_change.car_team_index];

                        generate_log_entry(CAR_STATE_CHANGE, car, NULL);

                        switch (car_state_change.new_state) {
                            case RACE:
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
                                break;
                            case DISQUALIFIED:
                                SYNC
                                SYNC_CLOCK_VALLEY
                                shm->num_cars_on_track--;
                                notify_cond(&shm->sync_s.clock_valley_cond);
                                END_SYNC_CLOCK_VALLEY
                                END_SYNC

                                if (check_race_end()) {
                                    show_stats_table();
                                    return;
                                }

                                break;
                            case FINISH:
                                if (car_state_change.prev_state != FINISH) {
                                    SYNC
                                    SYNC_CLOCK_VALLEY
                                    shm->num_cars_on_track--;
                                    notify_cond(&shm->sync_s.clock_valley_cond);
                                    END_SYNC_CLOCK_VALLEY
                                    END_SYNC
                                }

                                if (!race_winner) {
                                    generate_log_entry(CAR_RACE_WIN, car, NULL);

                                    race_winner = true;
                                }

                                if (check_race_end()) {
                                    show_stats_table();
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

static void initialize_team_slots(int num_teams) {
    int i;

    for(i = 0; i < num_teams; i++) {
        shm->race_teams[i].team_id = -1;
    }
}

static int check_race_end() {
    int j, k;
    race_box_t * box = NULL;
    race_team_t * team = NULL;
    race_car_t * car = NULL;

    if (++shm->num_finished_cars == shm->total_num_cars) {
        shm->sync_s.race_running = false;

        DEBUG_MSG(RACE_END, EVENT, "")

        j = 0;

        while (j < config.num_teams) { // notify all the boxes that are waiting for a new car/reservation that the race has finished.
            team = &shm->race_teams[j];
            box = &team->team_box;

            SYNC_BOX_COND
            notify_cond_all(&box->cond);
            END_SYNC_BOX_COND

            k = 0;

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

void create_team(char * team_name, int * team_id) {
    int i;
    race_team_t * team = NULL;

    i = 0;

    while (i < config.num_teams) {
        if (shm->race_teams[i].team_id >= 0) {
            i++;
            continue;
        }
        break;
    }

    team = &shm->race_teams[i];

    strcpy(team->team_name, team_name);
    team->team_id = i;
    *team_id = i;
    num_registed_teams++;

    create_unn_pipe(unn_pipe_fds);
    pipe_fds[i + 1] = unn_pipe_fds[0];

    create_process(TEAM_MANAGER, team_manager, (void *) team);
    close_fd(unn_pipe_fds[1]);
}

void register_car(race_car_t * car, int team_id) {
    init_mutex(&car->access_mutex, true);
    init_mutex(&car->cond_mutex, true);
    init_cond(&car->cond, true);
    car->team = &shm->race_teams[team_id];
    car->team_index = shm->race_teams[team_id].num_cars++;
    shm->num_cars_on_track++;
    car->car_id = ++shm->total_num_cars;
    shm->race_cars[team_id][car->team->num_cars - 1] = * car;
}

// endregion public functions