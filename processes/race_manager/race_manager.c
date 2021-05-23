/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include <signal.h>
#include <errno.h>
#include "stdio.h"
#include "string.h"
#include "fcntl.h"
#include "unistd.h"
#include "../../util/process_manager/process_manager.h"
#include "../../util/global.h"
#include "../../util/file/file.h"
#include "../../ipcs/pipe/pipe.h"
#include "../../helpers//log_generator/log_generator.h"
#include "../team_manager/team_manager.h"
#include "race_manager.h"
#include "../../helpers/cmd_validator/cmd_validator.h"
#include "../../helpers/stats_helper/stats_helper.h"
#include "../../ipcs/sync/semaphore/sem.h"
#include "../../helpers/signal_manager/signal_manager.h"

// endregion dependencies

// region public functions

/**
 * @def initialize_team_slots
 * @brief Function that initializes all of the team slots.
 *
 * @param num_teams
 * Number of team slots.
 *
 */
static void initialize_team_slots(int num_teams);

/**
 * @def handle_named_pipe
 * @brief Function that allows the handling of the named pipe before the start of the race.
 */
static void handle_named_pipe();

/**
 * @def handle_all_pipes
 * @brief Function that allows the management of all of the pipes after the race has started.
 */
static void handle_all_pipes();

/**
 * @def notify_race_start
 * @brief Functions that signals the start of the race.
 */
static void notify_race_start();

/**
 * @def notify_sim_end
 * @brief Function that notifies that the simulation is to end.
 */
static void notify_sim_end();

/**
 * @def register_car
 * @brief Function that registers a new race car on the shared memory region.
 *
 * @param car
 * Car to registered.
 *
 * @param team_id
 * Team ID assigned to the car.
 *
 */
static void register_car(race_car_t * car, int team_id);

/**
 * @def reset_race
 * @brief Function that resets the race.
 */
static void reset_race();

/**
 * @def restart_teams
 * @brief Function that restarts the race teams.
 */
static void restart_teams();

/**
 * @def check_race_end
 * @brief Function that check if the race has ended.
 *
 * @return false if the race has not ended.
 *         true if the race has ended.
 */
static int check_race_end();

//endregion public functions prototypes

int pipe_fds[MAX_NUM_TEAMS + 1], num_registered_teams = 0;

void race_manager(){

    DEBUG_MSG(PROCESS_RUN, ENTRY, RACE_MANAGER_SAYS, getpid());

    signal(SIGSEGV, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGUSR1, signal_handler);
    initialize_team_slots(config.num_teams);
    pipe_fds[NAMED_PIPE_INDEX] = open_file(RACE_SIMULATOR_NAMED_PIPE, O_RDONLY | O_NONBLOCK);

    do {
        shm->state = NOT_STARTED;
        //handle the named pipe to receive the commands
        handle_named_pipe();

        //check if the simulation is to end before starting the race
        if (shm->state == CLOSED) {
            end_clock();
            notify_sim_end();
            wait_procs();

            break;
        }

        // TODO: finish this comment
        if (shm->hold_on_end) {
            shm->hold_on_end = false;
            restart_teams();
            unpause_clock();
            reset_race();
        }

        notify_race_start();
        handle_all_pipes();

        wait_procs();

    } while(shm->hold_on_end);

    DEBUG_MSG(PROCESS_EXIT, ENTRY, RACE_MANAGER_SAYS, getpid())
}

void notify_race_start() {
    SYNC
    shm->state = RUNNING;
    notify_cond_all(&shm->cond);
    END_SYNC
}

void create_new_team(char * team_name, int * team_id) {
    int i;
    race_team_t * team = NULL;

    i = 0;

    //find the slot for the team
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
    num_registered_teams++;

    //create unnamed pipe for communication between team and race manager
    create_unn_pipe(unn_pipe_fds);
    pipe_fds[i + 1] = unn_pipe_fds[0];

    create_process(TEAM_MANAGER, (void (*)(void *)) team_manager, (void *) team);
    close_fd(unn_pipe_fds[1]);
}

static void handle_named_pipe() {
    int n, result, team_id;
    char buffer[LARGEST_SIZE * 4], buffer2[LARGEST_SIZE * 4], * aux, * aux2;
    race_car_t car_data;

    while (true) {
        do {
            //read the command or sequence of commands
            n = (int) read(pipe_fds[NAMED_PIPE_INDEX], buffer, LARGEST_SIZE * 4 * sizeof(char));

            if (n > 0) {
                buffer[n - 1]= '\0';
                aux = strtok_r(buffer, DELIM_3, &aux2);

                do {
                    strcpy(buffer2, aux);
                    result = interpret_command(aux, &car_data, &team_id);

                    //generate a response for the interpretation of the command
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
                            return;
                        case RESULT_CANNOT_START_RACE:
                            generate_log_entry(RACE_CANNOT_START, buffer2, NULL);
                            break;
                        case RESULT_EXIT:
                            generate_log_entry(EXIT_PROGRAM, NULL, NULL);
                            shm->state = CLOSED;
                            return;
                        case RESULT_HELP:
                            printf("%s\n", COMMANDS_HELP);
                            break;
                        default:
                            generate_log_entry(COMMAND_REJECT, buffer2, NULL);
                            break;
                    }
                } while ((aux = strtok_r(NULL, DELIM_3, &aux2)) != NULL);
            }
        } while (n > 0);
    }
}

// region private functions

static void handle_all_pipes() {
    fd_set read_set;
    int i, n, race_winner = false;
    char buffer[4 * LARGEST_SIZE];
    race_car_state_change_t car_state_change = {0};
    race_car_t * car = NULL;
    race_team_t * team = NULL;
    race_box_t * box = NULL;

    while (true) {
        FD_ZERO(&read_set);

        for (i = 0; i < config.num_teams + 1; i++) {
            FD_SET(pipe_fds[i], &read_set);
        }

        //select the pipe to read
        if (select(pipe_fds[config.num_teams] + 1, &read_set, NULL, NULL, NULL) > 0) {
            for (i = 0; i < config.num_teams + 1; i++) {
                if (FD_ISSET(pipe_fds[i], &read_set)) {
                    if (i == NAMED_PIPE_INDEX) {
                        do {
                            n = (int) read(pipe_fds[i], buffer,4 * LARGE_SIZE);
                            if (n > 0) {
                                buffer[n - 1] = '\0';
                                generate_log_entry(COMMAND_REJECT2, (void *) buffer, NULL);
                            }
                        } while (n > 0);

                        close(pipe_fds[i]);
                        pipe_fds[NAMED_PIPE_INDEX] = open_file(RACE_SIMULATOR_NAMED_PIPE, O_RDONLY | O_NONBLOCK);
                    } else {
                        read_stream(pipe_fds[i], (void *) &car_state_change, sizeof(race_car_state_change_t));
                        car = &shm->race_cars[car_state_change.team_id][car_state_change.car_team_index];
                        box = &car->team->team_box;
                        team = car->team;

                        generate_log_entry(CAR_STATE_CHANGE, car, NULL);

                        //handle the race cars state changes
                        switch (car_state_change.new_state) {
                            case RACE:
                                //everytime a car goes to the box it gets its fuel deposit filled
                                if (car_state_change.prev_state == IN_BOX) {
                                    SYNC
                                    shm->num_refuels++;
                                    END_SYNC
                                }

                                break;
                            case SAFETY:
                                //notify the team box that another car is on safety mode
                                SYNC_BOX_COND
                                team->num_cars_safety++;
                                notify_cond(&box->cond);
                                END_SYNC_BOX_COND

                                //register that a malfunction has occurred
                                if (car_state_change.malfunctioning) {
                                    SYNC
                                    shm->num_malfunctions++;
                                    END_SYNC
                                }

                                break;

                            case IN_BOX:
                                if (car_state_change.prev_state == SAFETY) {
                                    team->num_cars_safety--; // does not need sync since the box will know this change in the next loop iteration.
                                }
                                break;
                            case DISQUALIFIED:
                                //notify that box that is not needed anymore
                                if (car_state_change.prev_state == SAFETY) {
                                    SYNC_BOX_COND
                                    team->num_cars_safety--;
                                    notify_cond(&box->cond);
                                    END_SYNC_BOX_COND
                                }

                                SYNC
                                SYNC_CLOCK_VALLEY
                                shm->num_cars_on_track--;
                                notify_cond(&shm->thread_clock.clock_valley_cond);
                                END_SYNC_CLOCK_VALLEY
                                END_SYNC

                                if (check_race_end()) {
                                    return;
                                }

                                break;
                            case FINISH:

                                //notify that the box is not needed anymore
                                if (car_state_change.prev_state == SAFETY) {
                                    SYNC_BOX_COND
                                    team->num_cars_safety--;
                                    notify_cond(&box->cond);
                                    END_SYNC_BOX_COND
                                }

                                SYNC
                                SYNC_CLOCK_VALLEY
                                shm->num_cars_on_track--;
                                notify_cond(&shm->thread_clock.clock_valley_cond);
                                END_SYNC_CLOCK_VALLEY
                                END_SYNC

                                //check if it won the race
                                if (!race_winner) {
                                    generate_log_entry(CAR_RACE_WIN, car, NULL);

                                    race_winner = true;
                                }

                                if (check_race_end()) {
                                    return;
                                }

                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }
}

static void notify_sim_end() {
    SYNC
    shm->state = CLOSED;
    notify_cond_all(&shm->cond);
    END_SYNC
}

static int check_race_end() {
    if (++shm->num_finished_cars == shm->total_num_cars) {
        shm->state = FINISHED;
        notify_race_state_change();

        //check if the race is to be restarted later
        if (shm->hold_on_end) {
            pause_and_restart_clock();
        } else {
            end_clock();
        }

        generate_log_entry(RACE_FINISH, NULL, NULL);
        show_stats_table();

        return true;
    }

    return false;
}

static void initialize_team_slots(int num_teams) {
    int i;

    for(i = 0; i < num_teams; i++) {
        shm->race_teams[i].team_id = -1;
    }
}

static void restart_teams() {
    int i = 0;
    race_team_t  * team = NULL;

    while (i < config.num_teams) {
        create_unn_pipe(unn_pipe_fds);
        pipe_fds[i + 1] = unn_pipe_fds[0];

        team = &shm->race_teams[i];

        create_process(TEAM_MANAGER, team_manager, (void *) team);
        close_fd(unn_pipe_fds[1]);

        i++;
    }
}

static void register_car(race_car_t * car, int team_id) {
    init_mutex(&car->access_mutex, true);
    init_mutex(&car->cond_mutex, true);
    init_cond(&car->cond, true);
    car->team = &shm->race_teams[team_id];
    car->team_index = shm->race_teams[team_id].num_cars++;
    shm->num_cars_on_track++;
    car->car_id = ++shm->total_num_cars;
    shm->race_cars[team_id][car->team->num_cars - 1] = * car;
}

static void reset_race() {
    int i, j;
    race_team_t * team = NULL;
    race_car_t  * car = NULL;

    i = 0;

    shm->num_finished_cars = 0;
    shm->num_cars_on_track = shm->total_num_cars;
    shm->num_refuels = 0;
    shm->num_malfunctions = 0;

    while (i < config.num_teams) {
        j = 0;
        team = &shm->race_teams[i];

        while (j < team->num_cars) {
            car = &shm->race_cars[i][j];


            car->completed_laps = 0;
            car->remaining_fuel = config.fuel_tank_capacity;
            car->num_malfunctions = 0;
            car->num_refuels = 0;
            car->num_box_stops = 0;

            j++;
        }

        i++;
    }
}

// endregion private functions