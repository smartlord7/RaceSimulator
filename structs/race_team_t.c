#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../global.h"
#include "../util/error_handler.h"

race_team_t * race_team(const char * team_name) {
    race_team_t * new;

    if ((new = (race_team_t *) calloc(1, sizeof(race_team_t))) == NULL) {
        throw_error_end_exit(ERROR_MEMORY_ALLOCATION, RACE_TEAM);
    }

    strcpy(new->team_name, team_name);

    return new;
}

char * race_team_to_string(race_team_t * team) {
    if (team == NULL) {
        return NULL;
    }

    char * buffer = (char *) malloc(MAX_BUFFER_SIZE * sizeof(char));

    snprintf(buffer, MAX_BUFFER_SIZE * sizeof(char), "TEAM: %s ",
                                                    team->team_name);

    return buffer;
}