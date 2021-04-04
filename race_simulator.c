/*
 * Authors:
 *  - Joao Filipe Guiomar Artur, 2019217853
 *  - Sancho Amaral Simoes, 2019217590
 *
 * Date of creation: 02/04/2021
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "global.h"
#include "structs/race_config_t.h"
#include "util/ipc_manager.h"
#include "race_config_reader.h"
#include "race_manager.h"
#include "malfunction_manager.h"
#include "log_generator.h"
#include "util/process_manager.h"
#include "util/exception_handler.h"

#define CONFIG_FILE_NAME "config.txt"
#define LOG_FILE_NAME "log.txt"
#define MAX_CMD_SIZE 20

/**
 * Create the mechanisms needed for interprocess communication.
 * Includes the creation of shared memory zone, POSIX named semaphores.
 * @param num_teams Number of teams competing on the race.
 */
void create_ipcs(int num_teams);

/**
 * Destroys the mechanisms for interprocess communication.
 * Includes the destruction of shared memory zone and POSIX named semaphores.
 * @param num_teams Number of teams competing on the race.
 */
void destroy_ipcs(int num_teams);

/**
 * Terminate of all the processes of the group.
 */
void terminate();

/**
 * Main function of the application. Simulates the behavior of the race simulator.
 * @return Exit value.
 */

int shm_id;
shared_memory_t * mem_struct;
sem_t * output_mutex, * shm_mutex, * race_start, * malfunction_mng_start, ** boxes_availability;

int main() {
    //initialize debugging and exception handling mechanisms
    exc_handler_init(NULL, terminate, NULL);
    debug_init(NULL);

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

    mem_struct->cfg = cfg; // There are no other application processes, no MUTEX needed.

    log_init(LOG_FILE_NAME, output_mutex);
    generate_log_entry(I_SIMULATION_START, NULL);

    //create race manager process
    create_process(RACE_MANAGER, race_manager, NULL);

    //create malfuncion manager process
    create_process(MALFUNCTION_MANAGER, malfunction_manager, NULL);

    //wait for all of the child processes
    wait_all();

    generate_log_entry(I_SIMULATION_END, NULL);

    //destroy interprocess communication mechanisms
    destroy_ipcs(cfg->num_teams);

    //release allocated memory for configs
    free(cfg);

    DEBUG_MSG(EXITING_PROCESS, RACE_SIMULATOR)

    return EXIT_SUCCESS;
}

void create_ipcs(int num_teams){
    assert(num_teams > 0);

    mem_struct = create_shm(sizeof(shared_memory_t), &shm_id);

    output_mutex = create_sem(OUTPUT_MUTEX, 1);

    DEBUG_MSG(SEM_CREATED, OUTPUT_MUTEX)

    exc_mutex = output_mutex;

    deb_mutex = output_mutex;

    shm_mutex = create_sem(SHM_MUTEX, 1);

    DEBUG_MSG(SEM_CREATED, OUTPUT_MUTEX)

    race_start = create_sem(RACE_START, 0);

    DEBUG_MSG(SEM_CREATED, RACE_START)

    malfunction_mng_start = create_sem(MALFUNCTION_MNG_START, 0);

    DEBUG_MSG(SEM_CREATED, MALFUNCTION_MNG_START)

    boxes_availability = create_sem_array(num_teams, BOX_SEM_PREFIX, 1);
}

void destroy_ipcs(int num_teams){
    assert(num_teams > 0);

    destroy_shm(shm_id, mem_struct);

    deb_mutex = NULL;

    destroy_sem(OUTPUT_MUTEX, output_mutex);

    destroy_sem(SHM_MUTEX, shm_mutex);

    destroy_sem(RACE_START, race_start);

    destroy_sem(MALFUNCTION_MNG_START, malfunction_mng_start);

    destroy_sem_array(boxes_availability, num_teams, BOX_SEM_PREFIX);
}

void terminate() {
    pid_t proc_group_id = getpgrp();
    char cmd[MAX_CMD_SIZE];

    snprintf(cmd, MAX_CMD_SIZE, "pkill -9 -g %d", proc_group_id);

    system(cmd);
}