/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

// region dependencies

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../util/strings/strings.h"
#include "../../util/exception_handler/exception_handler.h"
#include "race_team_t.h"

// endregion dependencies

// region public functions

race_team_t * race_team(int team_id, const char * team_name) {
    race_team_t * new;

    if ((new = (race_team_t *) calloc(1, sizeof(race_team_t))) == NULL) {
        throw_and_exit(MEMORY_ALLOCATE_EXCEPTION, RACE_TEAM);
    }

    strcpy(new->team_name, team_name);
    new->team_id = team_id;

    return new;
}

char * race_team_to_string(race_team_t * team) {
    char * buffer = NULL;

    if (team == NULL) {
        buffer = string(NULL_STR_SIZE);

        snprintf(buffer, NULL_STR_SIZE * sizeof(char), "NULL");
    } else {
        buffer = string(BUF_SIZE);

        snprintf(buffer, BUF_SIZE * sizeof(char), "\nTEAM: %s\n", team->team_name);
    }

    return buffer;
}

char * race_box_state_to_string(box_state_t state) {
    char * buffer = NULL;

    buffer = string(BUF_SIZE);

    switch(state) {
        case FREE:
            snprintf(buffer, BUF_SIZE, "FREE");
            break;
        case RESERVED:
            snprintf(buffer, BUF_SIZE, "RESERVED");
            break;
        case OCCUPIED:
            snprintf(buffer, BUF_SIZE, "OCCUPIED");
            break;
        default:
            break;
    }

    return buffer;
}

// endregion public functions