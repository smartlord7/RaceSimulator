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
#include "global.h"
#include "log_generator.h"
#include "race_config_reader.h"
#include "race_manager.h"
#include "malfunction_manager.h"
#include "ipc_manager.h"
#include "process_manager.h"
#include "exception_handler.h"

// endregion dependencies

// region constants

#define CONFIG_FILE_NAME "config.txt"
#define LOG_FILE_NAME "log.txt"
#define RACE_SIMULATOR "RACE_SIMULATOR"
#define BOX_SEM_PREFIX "BOX_MUTEX_"

// endregion constants

// region private functions prototypes

/**
 * @def create_ipcs
 * @brief Function that creates the required IPCs for the application execution. Includes the creation of shared memory zone and POSIX named semaphores.
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


// endregion private functions prototypes

// region global variables

int shm_id;
shared_memory_t * shm = NULL;
sem_t * output_mutex = NULL,
        * shm_mutex = NULL,
        * race_start = NULL,
        * malfunction_mng_start = NULL,
        ** boxes_availability = NULL;

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

    DEBUG_MSG(IPCS_CREATED, "")

    shm->cfg = cfg; // There are no other application processes, no MUTEX needed.

    log_init(LOG_FILE_NAME, output_mutex);
    generate_log_entry(I_SIMULATION_START, NULL);

    //create race manager process
    create_process(RACE_MANAGER, race_manager, NULL);

    //create malfuncion manager process
    create_process(MALFUNCTION_MANAGER, malfunction_manager, NULL);

    //wait for all of the child processes
    wait_procs();

    generate_log_entry(I_SIMULATION_END, NULL);

    //destroy interprocess communication mechanisms
    destroy_ipcs(cfg->num_teams);

    //release allocated memory for configs
    free(cfg);

    DEBUG_MSG(EXITING_PROCESS, RACE_SIMULATOR)

    return EXIT_SUCCESS;
}

// region private functions

static void create_ipcs(int num_teams){
    assert(num_teams > 0);

    shm = create_shm(sizeof(shared_memory_t), &shm_id);

    output_mutex = create_sem(OUTPUT_MUTEX, 1);

    DEBUG_MSG(SEM_CREATED, OUTPUT_MUTEX)

    shm_mutex = create_sem(SHM_MUTEX, 1);

    DEBUG_MSG(SEM_CREATED, OUTPUT_MUTEX)

    race_start = create_sem(RACE_START, 0);

    DEBUG_MSG(SEM_CREATED, RACE_START)

    malfunction_mng_start = create_sem(MALFUNCTION_MNG_START, 0);

    DEBUG_MSG(SEM_CREATED, MALFUNCTION_MNG_START)

    boxes_availability = create_sem_array(num_teams, BOX_SEM_PREFIX, 1);
}

static void destroy_ipcs(int num_teams){
    assert(num_teams > 0);

    destroy_shm(shm_id, shm);

    destroy_sem(OUTPUT_MUTEX, output_mutex);

    destroy_sem(SHM_MUTEX, shm_mutex);

    destroy_sem(RACE_START, race_start);

    destroy_sem(MALFUNCTION_MNG_START, malfunction_mng_start);

    destroy_sem_array(boxes_availability, num_teams, BOX_SEM_PREFIX);
}

static void terminate() {
    terminate_proc_grp(getpgrp());

    // TODO Remove IPCs on abrupt termination.
}

// endregion private functions