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
#include "../../util/strings/strings.h"
#include "../../util/file/file.h"
#include "../../ipcs/pipe/pipe.h"
#include "../../race_helpers//log_generator/log_generator.h"
#include "../team_manager/team_manager.h"
#include "race_manager.h"
#include "../../race_helpers/cmd_validator/cmd_validator.h"

// endregion dependencies

// region public functions

static void create_teams(int num_teams);
static void handle_named_pipe();
static void handle_all_pipes();
static void notify_race_start();
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

                                if (check_race_end()) {
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
                                    DEBUG_MSG(CAR_WIN, EVENT, car_state_change.car_id, car_state_change.team_id, shm->sync_s.global_time)

                                    race_winner = true;
                                }

                                if (check_race_end()) {
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

// endregion public functions