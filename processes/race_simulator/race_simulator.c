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
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sys/fcntl.h>
#include "../../util/global.h"
#include "../../util/log_generator/log_generator.h"
#include "../../util/race_config_reader/race_config_reader.h"
#include "../race_manager/race_manager.h"
#include "../malfunction_manager/malfunction_manager.h"
#include "../../util/process_manager/process_manager.h"
#include "../../util/exception_handler/exception_handler.h"
#include "../../ipcs/shared_memory/shm.h"
#include "../../ipcs/sync/semaphore/sem.h"
<<<<<<< Updated upstream
#include "../../ipcs/sync/monitor/monitor.h"
=======
#include "../../ipcs/sync/condition_variable/cond_var.h"
#include "../../ipcs/pipe/pipe.h"
>>>>>>> Stashed changes
#include "../../ipcs/sync/mutex/mutex.h"

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
static void terminate();

void handle_named_pipe(int * fd);

void watch_named_pipe(void * fd);

void print_stats(void);

// endregion private functions prototypes

// region global variables

int shm_id, fd_named_pipe;
pthread_t npipe_thread_id;
shared_memory_t * shm = NULL;
sem_t * shm_mutex,  ** boxes_availability = NULL;

// endregion global variables

/**
 * Main function of the application. Simulates the behavior of a race simulator.
 * @return the exit value.
 *
 */

int main() {

    //initialize debugging and exception handling mechanisms
    exc_handler_init(terminate, NULL);

    //initialize and read configuration file.
    race_config_reader_init(CONFIG_FILE_NAME);
    race_config_t * cfg = read_race_config();

    DEBUG_MSG(RACE_CONFIG_CREATED, "")

    #if DEBUG
    printf("%s", race_config_to_string(cfg));
    #endif

    //create interprocess communication mechanisms
    create_ipcs(cfg->num_teams);

    DEBUG_MSG(IPCS_CREATE, "")

    shm->cfg = cfg; // There are no other application processes, no MUTEX needed.
    shm->sync_s.start = false;

    log_init(LOG_FILE_NAME);
    generate_log_entry(I_SIMULATION_START, NULL);

    //create race manager process
    create_process(RACE_MANAGER, race_manager, NULL);

    //create malfuncion manager process
    create_process(MALFUNCTION_MANAGER, malfunction_manager, NULL);

    //handle SIGTSTP
    signal(SIGTSTP, (_sig_func_ptr) print_stats);

    // handle SIGINT
    signal(SIGINT, terminate);

    //wait for all of the child processes
    wait_procs();

    generate_log_entry(I_SIMULATION_END, NULL);

    //destroy interprocess communication mechanisms
    destroy_ipcs(cfg->num_teams);

    //release allocated memory for configs
    free(cfg);

    DEBUG_MSG(PROCESS_EXIT, RACE_SIMULATOR)

    return EXIT_SUCCESS;
}

// region private functions

static void create_ipcs(int num_teams){
    assert(num_teams > 0);

    shm = create_shm(sizeof(shared_memory_t), &shm_id);

    shm_mutex = create_sem(SHM_MUTEX, 1);

    boxes_availability = create_sem_array(num_teams, BOX_SEM_PREFIX, 1);

    monitor_init(&shm->sync_s.start, &shm->sync_s.cond, &shm->sync_s.mutex);

    set_shared_proc(&shm->sync_s.cond);

    DEBUG_MSG(COND_VAR_CREATE, RACE_START_COND_VAR)

<<<<<<< Updated upstream
    set_sh_mutex(&shm->sync_s.mutex);

    DEBUG_MSG(MUTEX_CREATE, THREAD_MUTEX)
=======
    //create named pipe
    create_named_pipe(PIPE_NAME, &fd_named_pipe, O_RDWR);
    //TODO: handle_named_pipe(&shm->fd_named_pipe); - create process to read input from user


    //set_sh_mutex(&shm->sync_s.mutex);
>>>>>>> Stashed changes
}

static void destroy_ipcs(int num_teams){
    assert(num_teams > 0);

    monitor_destroy();

    DEBUG_MSG(COND_VAR_DESTROY, RACE_START_COND_VAR)

    destroy_mutex(&shm->sync_s.mutex);

    DEBUG_MSG(MUTEX_DESTROY, THREAD_MUTEX)

    destroy_shm(shm_id, shm);

    destroy_sem(SHM_MUTEX, shm_mutex);

    destroy_sem_array(boxes_availability, num_teams, BOX_SEM_PREFIX);


    //DEBUG_MSG(COND_VAR_DESTROY, RACE_START_COND_VAR)
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
    terminate_proc_grp(getpgrp());

    // TODO Remove IPCs on abrupt termination.
}

// endregion private functions