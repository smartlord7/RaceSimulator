/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include "stdlib.h"
#include "unistd.h"
#include "assert.h"
#include "signal.h"
#include "stdio.h"
#include "../../util/global.h"
#include "../../util/log_generator/log_generator.h"
#include "../../util/race_config_reader/race_config_reader.h"
#include "../race_manager/race_manager.h"
#include "../malfunction_manager/malfunction_manager.h"
#include "../../util/process_manager/process_manager.h"
#include "../../util/exception_handler/exception_handler.h"
#include "../../ipcs/shared_memory/shm.h"
#include "../../ipcs/message_queue/msg_queue.h"
#include "../../ipcs/pipe/pipe.h"

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
static void create_ipcs(int num_teams);

/**
 * @def destroy_ipcs
 * @brief Function that destroys the previously created IPCs for the application execution.
 *
 * @param num_teams
 * The number of teams competing on the race.
 *
 */
static void destroy_ipcs(int num_teams);

/**
 * @def terminate
 * @brief Function that terminates the current process tree.
 *
 */

static void terminate();

static void show_stats(int signum);

static void segfault_handler(int signum);

// endregion private functions prototypes

// region global variables

void shm_init();

int shm_id, malfunction_q_id;
race_config_t config;
shared_memory_t * shm = NULL;

// endregion global variables

/**
 * Main function of the application. Simulates the behavior of a race simulator.
 * @return the exit value.
 *
 */

int main() {
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGSEGV, segfault_handler);

    //initialize debugging and exception handling mechanisms
    exc_handler_init(terminate, NULL);
    debug_init(EVENT, false);

    //initialize and read configuration file.
    race_config_reader_init(CONFIG_FILE_NAME);
    race_config_t * cfg = read_race_config();

    config = * cfg;

    free(cfg);

    //create interprocess communication mechanisms
    create_ipcs(config.num_teams);
    shm_init();

    log_init(LOG_FILE_NAME);
    generate_log_entry(I_SIMULATION_START, NULL);

    //create race manager process
    create_process(MALFUNCTION_MANAGER, malfunction_manager, NULL);

    //create malfunction_q_id manager process
    create_process(RACE_MANAGER, race_manager, NULL);

    //handle SIGTSTP
    signal(SIGTSTP, show_stats);

    // handle SIGINT
    signal(SIGINT, terminate);

    //wait for all of the child processes
    wait_procs();

    generate_log_entry(I_SIMULATION_END, NULL);

    //destroy interprocess communication mechanisms
    destroy_ipcs(config.num_teams);

    return EXIT_SUCCESS;
}

void shm_init() {

    shm->sync_s.race_running = false;
    shm->global_time = 0;
    shm->num_cars_on_track = 0;
    shm->num_malfunctions = 0;
    shm->num_refuels = 0;
    shm->total_num_cars = 0;

}

// region private functions

static void create_ipcs(int num_teams){
    assert(num_teams > 0);

    shm = (shared_memory_t *) create_shm(sizeof(shared_memory_t), &shm_id);
    create_named_pipe(RACE_SIMULATOR_NAMED_PIPE);
    init_cond(&shm->sync_s.cond, true);
    init_mutex(&shm->sync_s.mutex, true);
    malfunction_q_id = create_msg_queue();
}

static void destroy_ipcs(int num_teams){
    assert(num_teams > 0);

    destroy_mutex(&shm->sync_s.mutex);
    destroy_cond(&shm->sync_s.cond);
    destroy_shm(shm_id, shm);
    destroy_msg_queue(malfunction_q_id);
}

static void show_stats(int signum) {

}

static void segfault_handler(int signum) {
    printf("WELL... THAT ESCALATED QUICKLY...\n");
}

static void terminate() {
    force_exit = true;
    destroy_ipcs(config.num_teams);
    kill(getpid(), SIGKILL);

    exit(EXIT_FAILURE);

    // TODO Remove IPCs on abrupt termination.
}

// endregion private functions