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
#include <stdio.h>
#include <string.h>
#include "race_team_t.h"
#include "../util/strings.h"
#include "../util/exception_handler.h"

// endregion dependencies

// region constants

#define RACE_CONFIG "RACE_TEAM"
#define true 1
#define false 0
#define MAX_BUFFER_SIZE 512
#define NULL_STR_SIZE 5

// endregion constants

// region public functions

race_team_t * race_team(const char * team_name) {
    race_team_t * new;

    if ((new = (race_team_t *) calloc(1, sizeof(race_team_t))) == NULL) {
        throw_exception_and_exit(MEMORY_ALLOCATION_EXCEPTION, RACE_CONFIG);
    }

    strcpy(new->team_name, team_name);

    return new;
}

char * race_team_to_string(race_team_t * team) {
    char * buffer = NULL;

    if (team == NULL) {
        buffer = string(NULL_STR_SIZE);

        snprintf(buffer, NULL_STR_SIZE * sizeof(char), "NULL");
    } else {
        buffer = string(MAX_BUFFER_SIZE);

        snprintf(buffer, MAX_BUFFER_SIZE * sizeof(char), "TEAM: %s ", team->team_name);
    }

    return buffer;
}

// endregion public functions