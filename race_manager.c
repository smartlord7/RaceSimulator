#include "global.h"
#include <unistd.h>
#include <stdlib.h>
#include "util/error_handler.h"

#define TEAM "TEAM"

int start_team_manager();

int race_manager(){
    if(start_team_manager() == EXIT_FAILURE){
        throw_error_end_exit(ERROR_CREATE_PROCESS, TEAM);
        return EXIT_FAILURE;
    }
}

int start_team_manager(){
    pid_t child;

    child = fork();
    if(child < 0) return EXIT_FAILURE;
    else if(child == 0){
        team_manager();
        exit(EXIT_SUCCESS);
    }
    return EXIT_SUCCESS;
}