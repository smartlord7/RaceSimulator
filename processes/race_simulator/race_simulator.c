<<<<<<< Updated upstream
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
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sys/fcntl.h>
#include <signal.h>
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
static void terminate();

static void handle_named_pipe(int * fd);

static void watch_named_pipe(void * fd);

static void show_stats(void);

// endregion private functions prototypes

// region global variables

int shm_id, malfunction_msg_q_id, fd_named_pipe;
race_config_t config;
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
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    //initialize debugging and exception handling mechanisms
    exc_handler_init(terminate, NULL);

    //initialize and read configuration file.
    race_config_reader_init(CONFIG_FILE_NAME);
    race_config_t * cfg = read_race_config();

    config = * cfg;
    free(cfg);

    //create interprocess communication mechanisms
    create_ipcs(config.num_teams);

    shm->sync_s.start = false;

    log_init(LOG_FILE_NAME);
    generate_log_entry(I_SIMULATION_START, NULL);

    //create race manager process
    create_process(RACE_MANAGER, race_manager, NULL);

    //create malfuncion manager process
    create_process(MALFUNCTION_MANAGER, malfunction_manager, NULL);

    //handle SIGTSTP
    signal(SIGTSTP, (_sig_func_ptr) show_stats);

    // handle SIGINT
    signal(SIGINT, terminate);

    // test start_cond var
    sleep(1);

    change_condition(true, RACE_START_MONITOR);

    //wait for all of the child processes
    wait_procs();

    generate_log_entry(I_SIMULATION_END, NULL);

    //destroy interprocess communication mechanisms
    destroy_ipcs(config.num_teams);

    return EXIT_SUCCESS;
}

// region private functions

static void create_ipcs(int num_teams){
    assert(num_teams > 0);

    shm = create_shm(sizeof(shared_memory_t), &shm_id);

    boxes_availability = create_sem_array(num_teams, BOX_SEM_PREFIX, 1);

    monitor_init(&shm->sync_s.start, &shm->sync_s.start_cond, &shm->sync_s.start_mutex, true);
    monitor_init(&shm->sync_s.global_timer_start, &shm->sync_s.global_timer_start_cond, &shm->sync_s.global_timer_start_mutex, true);
    monitor_init(&shm->sync_s.global_timer_end, &shm->sync_s.global_timer_end_cond, &shm->sync_s.global_timer_end_mutex, true);

    create_named_pipe(PIPE_NAME, &fd_named_pipe, O_RDONLY);


    //set_sh_mutex(&shm->sync_s.mutex);

    malfunction_msg_q_id = create_msg_queue();
}

static void destroy_ipcs(int num_teams){
    assert(num_teams > 0);

    monitor_destroy_all();

    destroy_shm(shm_id, shm);

    destroy_sem_array(boxes_availability, num_teams, BOX_SEM_PREFIX);

    destroy_msg_queue(malfunction_msg_q_id);
}

static void show_stats() {

}

static void terminate() {
    terminate_proc_grp(getpgrp());

    // TODO Remove IPCs on abrupt termination.
}

=======
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
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sys/fcntl.h>
#include <signal.h>
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
static void terminate();

static void handle_named_pipe(int * fd);

static void watch_named_pipe(void * fd);

static void show_stats(void);

// endregion private functions prototypes

// region global variables

int shm_id, malfunction_msg_q_id, fd_named_pipe;
race_config_t config;
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
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    //initialize debugging and exception handling mechanisms
    exc_handler_init(terminate, NULL);

    //initialize and read configuration file.
    race_config_reader_init(CONFIG_FILE_NAME);
    race_config_t * cfg = read_race_config();

    config = * cfg;
    free(cfg);

    //create interprocess communication mechanisms
    create_ipcs(config.num_teams);

    shm->sync_s.start = false;

    log_init(LOG_FILE_NAME);
    generate_log_entry(I_SIMULATION_START, NULL);

    //create race manager process
    create_process(RACE_MANAGER, race_manager, NULL);

    //create malfuncion manager process
    create_process(MALFUNCTION_MANAGER, malfunction_manager, NULL);

    //handle SIGTSTP
    signal(SIGTSTP, (_sig_func_ptr) show_stats);

    // handle SIGINT
    signal(SIGINT, terminate);

    // test start_cond var
    sleep(1);

    change_condition(true, RACE_START_MONITOR);

    //wait for all of the child processes
    wait_procs();

    generate_log_entry(I_SIMULATION_END, NULL);

    //destroy interprocess communication mechanisms
    destroy_ipcs(config.num_teams);

    return EXIT_SUCCESS;
}

// region private functions

static void create_ipcs(int num_teams){
    assert(num_teams > 0);

    shm = create_shm(sizeof(shared_memory_t), &shm_id);

    boxes_availability = create_sem_array(num_teams, BOX_SEM_PREFIX, 1);

    monitor_init(&shm->sync_s.start, &shm->sync_s.start_cond, &shm->sync_s.start_mutex, true);
    monitor_init(&shm->sync_s.global_timer_start, &shm->sync_s.global_timer_start_cond, &shm->sync_s.global_timer_start_mutex, true);
    monitor_init(&shm->sync_s.global_timer_end, &shm->sync_s.global_timer_end_cond, &shm->sync_s.global_timer_end_mutex, true);

    create_named_pipe(PIPE_NAME, &fd_named_pipe, O_RDONLY);


    //set_sh_mutex(&shm->sync_s.mutex);

    malfunction_msg_q_id = create_msg_queue();
}

static void destroy_ipcs(int num_teams){
    assert(num_teams > 0);

    monitor_destroy_all();

    destroy_shm(shm_id, shm);

    destroy_sem_array(boxes_availability, num_teams, BOX_SEM_PREFIX);

    destroy_msg_queue(malfunction_msg_q_id);
}

static void show_stats() {

}

static void terminate() {
    terminate_proc_grp(getpgrp());

    // TODO Remove IPCs on abrupt termination.
}

>>>>>>> Stashed changes
// endregion private functions