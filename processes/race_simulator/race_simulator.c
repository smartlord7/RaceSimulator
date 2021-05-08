/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include "../../util/global.h"
#include "../../util/log_generator/log_generator.h"
#include "../../util/race_config_reader/race_config_reader.h"
#include "../race_manager/race_manager.h"
#include "../malfunction_manager/malfunction_manager.h"
#include "../../util/process_manager/process_manager.h"
#include "../../util/exception_handler/exception_handler.h"
#include "../../ipcs/shared_memory/shm.h"
#include "../../ipcs/sync/semaphore/sem.h"
#include "../../ipcs/message_queue/msg_queue.h"
#include "../../ipcs/pipe/pipe.h"

// endregion dependencies

// region constants

#define CONFIG_FILE_NAME "config.txt"
#define LOG_FILE_NAME "log.txt"
#define RACE_SIMULATOR "RACE_SIMULATOR"
#define BOX_SEM_PREFIX "BOX_MUTEX_"
#define PIPE_NAME "PIPE_NAME"


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

static void notify_start_race();

static void terminate();

static void handle_named_pipe(int * fd);

static void watch_named_pipe(void * fd);

static void show_stats(int signum);

// endregion private functions prototypes

// region global variables

int shm_id, malfunction_q_id, fd_named_pipe;
race_config_t config;
pthread_t npipe_thread_id;
shared_memory_t * shm = NULL;
sem_t ** boxes_availability = NULL;

// endregion global variables

/**
 * Main function of the application. Simulates the behavior of a race simulator.
 * @return the exit value.
 *
 */

int main() {
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    //initialize debugging and exception handling mechanisms
    exc_handler_init(terminate, NULL);
    debug_init(DEBUG_LEVEL_EVENT, false);

    //initialize and read configuration file.
    race_config_reader_init(CONFIG_FILE_NAME);
    race_config_t * cfg = read_race_config();

    config = * cfg;

    free(cfg);

    //create interprocess communication mechanisms
    create_ipcs(config.num_teams);

    shm->sync_s.race_start = false;

    log_init(LOG_FILE_NAME);
    generate_log_entry(I_SIMULATION_START, NULL);

    //create race manager process
    create_process(RACE_MANAGER, race_manager, NULL);

    //create malfunction_q_id manager process
    create_process(MALFUNCTION_MANAGER, malfunction_manager, NULL);

    //handle SIGTSTP
    //signal(SIGTSTP, show_stats);

    // handle SIGINT
    signal(SIGINT, terminate);

    notify_start_race();

    //wait for all of the child processes
    wait_procs();

    generate_log_entry(I_SIMULATION_END, NULL);

    //destroy interprocess communication mechanisms
    destroy_ipcs(config.num_teams);

    return EXIT_SUCCESS;
}

// region private functions

static void notify_start_race() {
    sleep(2);

    int i, j;
    for (i = 0; i < config.num_teams; i++) {
        for (j = 0; j < shm->race_teams[i].num_cars; j++) {
            race_car_t * current_car = &shm->race_cars[i][j];

            lock_mutex(&current_car->mutex);

            notify_cond(&current_car->start_cond);

            unlock_mutex(&current_car->mutex);
        }
    }

    notify_cond(&shm->sync_s.start_cond);
}

static void create_ipcs(int num_teams){
    assert(num_teams > 0);

    shm = (shared_memory_t *) create_shm(sizeof(shared_memory_t), &shm_id);
    malfunction_q_id = create_msg_queue();
}

static void destroy_ipcs(int num_teams){
    assert(num_teams > 0);

    destroy_shm(shm_id, shm);
    destroy_msg_queue(malfunction_q_id);
}

static void show_stats(int signum) {

}

void handle_named_pipe(int * fd) {

    if(pthread_create(&npipe_thread_id, NULL, (void *) watch_named_pipe, (void *) fd) != 0) {
        throw_and_exit(THREAD_CREATE_EXCEPTION, "NAMED PIPE HANDLER");
    }

}

void watch_named_pipe(void * fd) {
    //TODO: read input from user and redirect to named pipe
}

static void terminate() {
    //destroy_ipcs(config.num_teams);
    kill(getpid(), SIGKILL);

    exit(EXIT_FAILURE);

    // TODO Remove IPCs on abrupt termination.
}

// endregion private functions