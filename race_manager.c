#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "util/error_handler.h"
#include "util/process_manager.h"
#include "util/debug.h"

void race_manager(void * data){
    DEBUG_MSG(RUNNING_PROCESS, RACE_MANAGER);

    int num_teams = mem_struct->cfg->num_teams, i = 0;
    race_team_t * teams;

    if ((teams = (race_team_t *) malloc(num_teams * sizeof(race_team_t))) == NULL) {
        throw_error_end_exit(ERROR_MEMORY_ALLOCATION, "TEAMS LIST!");
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

    exit(EXIT_SUCCESS);
}
