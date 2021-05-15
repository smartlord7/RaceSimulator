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
#include "signal.h"
#include "../../util/global.h"
#include "../../race_helpers//log_generator/log_generator.h"
#include "../../race_helpers//race_config_reader/race_config_reader.h"
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

    // TODO: signal before race starts
    // TODO: handle multiple access in named pipe

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
    create_process(RACE_MANAGER, race_manager, NULL);

    //create malfunction_q_id manager process
    create_process(MALFUNCTION_MANAGER, malfunction_manager, NULL);

    //handle SIGTSTP
    signal(SIGTSTP, show_stats);

    // handle SIGINT
    signal(SIGINT, terminate);

    //wait for all of the child processes
    wait_procs();

    generate_log_entry(I_SIMULATION_END, NULL);

    //destroy interprocess communication mechanisms
    destroy_ipcs();

    return EXIT_SUCCESS;
}

void shm_init() {
    shm->sync_s.race_running = false;
    shm->total_num_cars = 0;
    shm->num_cars_on_track = 0;
    shm->num_finished_cars = 0;
    shm->num_malfunctions = 0;
    shm->num_refuels = 0;
    shm->global_time = 0;
}

// region private functions

static void create_ipcs(){
    shm = (shared_memory_t *) create_shm(sizeof(shared_memory_t), &shm_id);
    init_cond(&shm->sync_s.cond, true);
    init_mutex(&shm->sync_s.mutex, true);
    init_mutex(&shm->sync_s.malf_mutex, true);
    init_cond(&shm->sync_s.malfunction_mng_start, true);
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
    destroy_mutex(&shm->sync_s.mutex);
    destroy_cond(&shm->sync_s.cond);

    for (i = 0; i < config.num_teams; i++) {

        team = &shm->race_teams[i];
        box = &team->team_box;

        destroy_mutex(&team->access_mutex);
        destroy_mutex(&team->pipe_mutex);

        destroy_mutex(&box->access_mutex);
        destroy_mutex(&box->cond_mutex);
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

static void show_stats(int signum) {

}

static void segfault_handler(int signum) {
    //printf("WELL... THAT ESCALATED QUICKLY...\n");
}

static void terminate() {
    destroy_ipcs();
    kill(getpid(), SIGKILL);

    exit(EXIT_FAILURE);

    // TODO Remove IPCs on abrupt termination.
}

// endregion private functions