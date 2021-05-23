/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include "stdlib.h"
#include "signal.h"
#include "unistd.h"
#include "../../util/global.h"
#include "../../helpers//log_generator/log_generator.h"
#include "../../helpers//race_config_reader/race_config_reader.h"
#include "../race_manager/race_manager.h"
#include "../malfunction_manager/malfunction_manager.h"
#include "../../util/process_manager/process_manager.h"
#include "../../util/exception_handler/exception_handler.h"
#include "../../ipcs/shared_memory/shm.h"
#include "../../ipcs/message_queue/msg_queue.h"
#include "../../ipcs/pipe/pipe.h"
#include "../../util/numbers/numbers.h"
#include "../../helpers/stats_helper/stats_helper.h"
#include "../../util/file/file.h"

// endregion dependencies

// region constants

#define CONFIG_FILE_NAME "config.txt"
#define LOG_FILE_NAME "log.txt"

// endregion constants

// region private functions prototypes

/**
 * @def create_ipcs
 * @brief Function that creates the required IPCs for the application execution. Includes the creation of shared memory zone and POSIX named semaphore.
 *
 * @param num_teams
 * The number of teams competing in the race.
 *
 */
static void create_ipcs();

/**
 * @def destroy_ipcs
 * @brief Function that destroys the previously created IPCs for the application execution.
 *
 * @param num_teams
 * The number of teams competing on the race.
 *
 */
static void destroy_ipcs();

/**
 * @def terminate
 * @brief Function that terminates the current process tree.
 *
 */
static void terminate();

/**
 * @def shm_init
 * @brief Function that initializes variables in the shared memory region.
 */
static void shm_init();

// endregion private functions prototypes

// region global variables

int shm_id, malfunction_q_id, ipcs_created;
race_config_t config;
shared_memory_t * shm = NULL;

// endregion global variables

/**
 * Main function of the application. Simulates the behavior of a race simulator.
 *
 * @return the exit value.
 */
int main() {
    DEBUG_MSG(PROCESS_RUN, ENTRY, RACE_SIMULATOR_PROCESS, getpid())

    signal(SIGSEGV, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGUSR1, SIG_IGN);

    //initialize debugging and exception handling mechanisms
    exc_handler_init((void (*)(void *)) terminate, NULL);
    debug_init(EVENT, false);

    // TODO: signal before race starts
    // TODO: handle multiple access in named pipe

    //initialize and read configuration file.
    race_config_reader_init(CONFIG_FILE_NAME);
    race_config_t * cfg = read_race_config();

    config = * cfg;

    free(cfg);

    //create interprocess communication mechanisms
    create_ipcs();
    shm_init();

    stats_helper_init(&config, shm, &shm->mutex);

    log_init(LOG_FILE_NAME);
    generate_log_entry(SIMULATION_START, NULL, NULL);

    //create race manager process
    create_process(RACE_MANAGER, (void (*)(void *)) race_manager, NULL);

    //create malfunction_q_id manager process
    create_process(MALFUNCTION_MANAGER, (void (*)(void *)) malfunction_manager, NULL);

    if (wait_race_start()) {
        init_global_clock();
    }

    // handle SIGINT

    //wait for all of the child processes
    wait_procs();
    end_clock();

    generate_log_entry(SIMULATION_END, NULL, NULL);

    //destroy interprocess communication mechanisms
    destroy_ipcs();

    DEBUG_MSG(PROCESS_EXIT, ENTRY, RACE_SIMULATOR_PROCESS, getpid())

    return EXIT_SUCCESS;
}

int wait_race_start() {
    SYNC
    while (shm->state != RUNNING && shm->state != CLOSED) {
        wait_cond(&shm->cond, &shm->mutex);
    }
    END_SYNC

    if (shm->state == RUNNING) {
        return true;
    }

    return false;
}

void notify_race_state_change() {
    int j;
    race_box_t * box = NULL;
    race_team_t * team = NULL;

    j = 0;

    SYNC
    notify_cond_all(&shm->cond);
    END_SYNC

    while (j < config.num_teams) { // notify all the boxes that are waiting for a new car/reservation that the race has finished.
        team = &shm->race_teams[j];
        box = &team->team_box;

        SYNC_BOX_COND
        notify_cond_all(&box->cond);
        END_SYNC_BOX_COND
        j++;
    }
}

void shm_init() {
    shm->thread_clock.clock_on = false;
    shm->hold_on_end = false;
    shm->thread_clock.num_clock_waiters = 0;
    shm->thread_clock.global_time = 0;
    shm->total_num_cars = 0;
    shm->num_cars_on_track = 0;
    shm->num_finished_cars = 0;
    shm->num_malfunctions = 0;
    shm->num_refuels = 0;
}

// region private functions

static void create_ipcs(){
    ipcs_created = true;

    shm = (shared_memory_t *) create_shm(sizeof(shared_memory_t), &shm_id);
    init_cond(&shm->cond, true);
    init_mutex(&shm->mutex, true);
    init_mutex(&shm->stdout_mutex, true);
    init_mutex(&shm->log_mutex, true);
    init_cond(&shm->thread_clock.clock_valley_cond, true);
    init_mutex(&shm->thread_clock.clock_valley_mutex, true);
    init_mutex(&shm->thread_clock.clock_rise_mutex, true);
    init_cond(&shm->thread_clock.clock_rise_cond, true);
    create_named_pipe(RACE_SIMULATOR_NAMED_PIPE);
    malfunction_q_id = create_msg_queue();
}

static void destroy_ipcs(){
    force_exit = true;

    int i, j;
    race_team_t * team = NULL;
    race_box_t  * box = NULL;
    race_car_t * car = NULL;

    destroy_msg_queue(malfunction_q_id);
    destroy_named_pipe(RACE_SIMULATOR_NAMED_PIPE);
    destroy_mutex(&shm->mutex);
    destroy_cond(&shm->cond);
    destroy_mutex(&shm->stdout_mutex);
    destroy_mutex(&shm->log_mutex);

    for (i = 0; i < config.num_teams; i++) {

        team = &shm->race_teams[i];
        box = &team->team_box;

        destroy_mutex(&team->pipe_mutex);

        destroy_mutex(&box->mutex);
        destroy_mutex(&box->available);

        for (j = 0; j < team->num_cars; j++) {
            car = &shm->race_cars[team->team_id][j];
            destroy_mutex(&car->access_mutex);
            destroy_mutex(&car->cond_mutex);
            destroy_cond(&car->cond);
        }
    }

    destroy_shm(shm_id, shm);
    log_close();
}

static void terminate() {
    if (ipcs_created) {
        destroy_ipcs();
    }

    terminate_proc_grp(getpgrp());

    exit(EXIT_FAILURE);

    // TODO Remove IPCs on abrupt termination.
}

// endregion private functions