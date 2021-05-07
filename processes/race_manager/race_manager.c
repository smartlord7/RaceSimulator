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
#include <sys/unistd.h>
#include "../../util/process_manager/process_manager.h"
#include "../../util/global.h"
#include "../team_manager/team_manager.h"
#include "race_manager.h"

// endregion dependencies

// region public functions

int get_named_pipe_fd();
int get_num_teams();
int read_command(int fd, char * buffer, int size);

int interpret_command(char *buffer, race_car_t *pCar, race_team_t *pTeam);

int n_pipe_fd;

void race_manager(){

    DEBUG_MSG(PROCESS_RUN, RACE_MANAGER);

    int num_teams = config.num_teams, i = 0;
    race_team_t * team = NULL;
    race_box_t * box = NULL;
    race_car_t * car = NULL;


    int n_pipe_fd = get_named_pipe_fd();
    int result_type;

    //create the teams' processes
    while (i < num_teams) {
        char team_name[MAX_LABEL_SIZE];
        snprintf(team_name, MAX_LABEL_SIZE * sizeof(char), "%s_%d", TEAM_MANAGER, i);

        team = &shm->race_teams[i];
        box = &shm->race_boxes[i];

        strcpy(team->team_name, team_name);
        team->team_id = i;
        team->team_box = box;
        box->state = FREE;
        box->box_availability = boxes_availability[i];
        box->team = team;

        create_process(TEAM_MANAGER, team_manager, (void *) team);
        i++;
    }

    // cria thread watch pipe

    // wait pela var final;
    char buffer[MAX_BUFFER_SIZE];
    while(true) {

        if(read_command(n_pipe_fd, buffer, sizeof(buffer))) {

            result_type = interpret_command(buffer, car, team);

            switch (result_type) {

            }

        }
    }



    //register

    //wait for all the child processes
    wait_procs();

    DEBUG_MSG(PROCESS_EXIT, RACE_MANAGER)

    exit(EXIT_SUCCESS);
}

int get_named_pipe_fd() {
    int aux;

    //get named pipe file descriptor in mutual exclusion
    sem_wait(shm_mutex);

    //aux = fd_named_pipe;

    sem_post(shm_mutex);

    return aux;
}

int get_num_teams() {
    int aux;

    //get named pipe file descriptor in mutual exclusion
    sem_wait(shm_mutex);

    aux = config.num_teams;

    sem_post(shm_mutex);

    return aux;
}

int read_command(int fd, char * buffer, int size){
    int n;

    do {
        n = read(fd, buffer, size);
    } while(n > 0);

    if(n < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int interpret_command(char *buffer, race_car_t *pCar, race_team_t *pTeam) {
    char command_preview[COMMAND_PREVIEW_SIZE];

    memcpy(command_preview, buffer, sizeof(command_preview) - sizeof(char));
    command_preview[COMMAND_PREVIEW_SIZE - 1] = '\0';

    if(strcasecmp(command_preview, START) == 0) {

    } else if(strcasecmp(command_preview, START) == 0) {

    } else return RESULT_NOT_RECO;
}

// endregion public functions