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
#include <string.h>
#include "global.h"
#include "team_manager.h"
#include "util/exception_handler.h"
#include "util/process_manager.h"

/**
 * Handle the concept of race manager and its respective tasks.
 */
void race_manager(){
    S_DEBUG_MSG(RUNNING_PROCESS, RACE_MANAGER);

    int num_teams = mem_struct->cfg->num_teams, i = 0;
    race_team_t * teams;

    if ((teams = (race_team_t *) malloc(num_teams * sizeof(race_team_t))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, "TEAMS LIST!");
    }

    mem_struct->race_teams = teams;

    while (i < num_teams) {
        char team_name[MAX_LABEL_SIZE];
        snprintf(team_name, MAX_LABEL_SIZE * sizeof(char), "%s_%d", RACE_TEAM, i);

        strcpy(teams[i].team_name, team_name);

        create_process(TEAM_MANAGER, team_manager, (void *) &teams[i]);
        i++;
    }

    wait_all();
    free(teams);

    S_DEBUG_MSG(EXITING_PROCESS, RACE_MANAGER)

    exit(EXIT_SUCCESS);
}