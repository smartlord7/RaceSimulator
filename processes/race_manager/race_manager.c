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
#include "../../util/process_manager/process_manager.h"
#include "../../util/global.h"
#include "../team_manager/team_manager.h"
#include "race_manager.h"

// endregion dependencies

// region public functions

void race_manager(){
    DEBUG_MSG(PROCESS_RUN, RACE_MANAGER);

    int num_teams = shm->cfg->num_teams, i = 0;
    race_team_t * team = NULL;
    race_box_t * box = NULL;

    //create the teams' processes
    while (i < num_teams) {
        char team_name[MAX_LABEL_SIZE];
        snprintf(team_name, MAX_LABEL_SIZE * sizeof(char), "%s_%d", TEAM_MANAGER, i);

        team = &shm->race_teams[i];
        box = &shm->race_boxes[i];

        strcpy(team->team_name, team_name);
        team->team_box = box;
        box->state = FREE;
        box->box_availability = boxes_availability[i];
        box->team = team;

        create_process(TEAM_MANAGER, team_manager, (void *) team);

        i++;
    }

    //wait for all the child processes
    wait_procs();

    DEBUG_MSG(PROCESS_EXIT, RACE_MANAGER)

    exit(EXIT_SUCCESS);
}
/*
int read_command(int fd, int * type, race_car_t * read_data){
    int n;
    char buffer[MAX_BUFFER_SIZE];


}*/

// endregion public functions