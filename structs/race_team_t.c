/*
 * Authors:
 *  - Joao Filipe Guiomar Artur, 2019217853
 *  - Sancho Amaral Simoes, 2019217590
 *
 * Date of creation: 02/04/2021
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "race_team_t.h"
#include "../util/exception_handler.h"

#define true 1
#define false 0
#define MAX_BUFFER_SIZE 512

race_team_t * race_team(const char * team_name) {
    race_team_t * new;

    if ((new = (race_team_t *) calloc(1, sizeof(race_team_t))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, "BUFFER");
    }

    strcpy(new->team_name, team_name);

    return new;
}

char * race_team_to_string(race_team_t * team) {
    if (team == NULL) {
        return NULL;
    }

    char * buffer;

    if ((buffer = (char *) malloc(MAX_BUFFER_SIZE * sizeof(char))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, "BUFFER");
    }

    snprintf(buffer, MAX_BUFFER_SIZE * sizeof(char), "TEAM: %s ",
                                                    team->team_name);

    return buffer;
}