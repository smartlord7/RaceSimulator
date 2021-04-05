/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "race_box_t.h"
#include "race_manager.h"
#include "team_manager.h"
#include "exception_handler.h"
#include "process_manager.h"

// endregion dependencies

// region public functions

void race_manager(){
    DEBUG_MSG(RUNNING_PROCESS, RACE_MANAGER);

    int num_teams = mem_struct->cfg->num_teams, i = 0;
    race_team_t * teams;
    race_box_t * boxes;

    //allocate memory space for the teams
    if ((teams = (race_team_t *) malloc(num_teams * sizeof(race_team_t))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, TEAMS);
    }

    if ((boxes = (race_box_t *) malloc(num_teams * sizeof(race_box_t))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, BOXES);
    }

    mem_struct->race_teams = teams;
    mem_struct->race_boxes = boxes;

    //create the teams' processes
    while (i < num_teams) {
        char team_name[MAX_LABEL_SIZE];
        snprintf(team_name, MAX_LABEL_SIZE * sizeof(char), "%s_%d", RACE_TEAM, i);

        strcpy(teams[i].team_name, team_name);
        teams[i].team_box = &boxes[i];
        boxes[i].state = FREE;
        boxes[i].box_availability = boxes_availability[i];
        boxes[i].team = &teams[i];

        create_process(TEAM_MANAGER, team_manager, (void *) &teams[i]);

        i++;
    }

    //wait for all the child processes
    wait_all();

    //free the teams pointer
    free(teams);

    DEBUG_MSG(EXITING_PROCESS, RACE_MANAGER)

    exit(EXIT_SUCCESS);
}

// endregion public functions