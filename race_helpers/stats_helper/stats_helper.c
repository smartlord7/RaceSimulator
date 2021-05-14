#include <assert.h>
#include "malloc.h"
#include "stdlib.h"
#include "string.h"
#include "stats_helper.h"
#include "../../structs/race_car/race_car_t.h"
#include "../../structs/shared_memory/shared_memory_t.h"
#include "../../util/strings/strings.h"

race_config_t * conf = NULL;
shared_memory_t * shm = NULL;

void init_stats_helper(race_config_t * cfg, shared_memory_t * shmem) {
    assert(cfg != NULL && shmem != NULL);

    conf = cfg;
    shm = shmem;
}

static void swap_car(race_car_t * car1, race_car_t * car2) {
    race_car_t * temp = car1;
    * car1 = * car2;
    * car2 = * temp;
}

static void selection_sort_cars_by_completed_laps(race_car_t * race_cars, int size) {
    int i, j, min_index;
    race_car_t car;

    for (i = 0; i < size - 1; i++) {
        min_index = i;
        car = race_cars[i];
        for (j = i + 1; j < size; j++) {
            if (race_cars[j].completed_laps > race_cars[min_index].completed_laps) {
                min_index = j;
            }

            swap_car(&race_cars[min_index], &race_cars[i]);
        }
    }
}

static int get_team_name_max_len(race_team_t * race_teams, int size) {
    assert(race_teams != NULL && size > 0);

    int max_len = -1, i = 0, len;

    while (i < size) {
        len = (int) strlen(race_teams[i].team_name);

        if (len > max_len) {
            max_len = len;
        }

        i++;
    }

    return max_len;
}

static int get_car_name_max_len(race_car_t * race_cars, int size) {
    assert(race_cars != NULL && size > 0);

    int max_len = -1, i = 0, len;

    while (i < size) {
        len = (int) strlen(race_cars[i].name);

        if (len > max_len) {
            max_len = len;
        }

        i++;
    }

    return max_len;
}

static race_car_t * get_all_cars(shared_memory_t * shm_cpy) {
    int i, j, k;
    race_car_t * cars = NULL;
    race_team_t * team = NULL;

    cars = (race_car_t *) malloc(shm->total_num_cars * sizeof(race_car_t));

    i = 0;
    k = 0;
    while (i < conf->num_teams) { //TODO: replace by config
        j = 0;
        team = &shm->race_teams[i];

        while (j < team->num_cars) {
            cars[k++] = shm->race_cars[i][j++];
        }

        i++;
    }

    return cars;
}

void show_race_stats_table() { // TODO: validate functions result
    char buffer[BUFFER_SIZE], aux[BUFFER_SIZE], * row_sep_half, * row_sep;
    shared_memory_t * shm_cpy = NULL;
    race_car_t * race_cars = NULL, * car = NULL;
    int i, max_team_name_len, max_car_name_len,
        max_car_name_col_width, max_team_name_col_width, row_width, title_length,
        team_id_col_width, car_laps_col_width, car_box_stops_col_width;

    max_team_name_len = get_team_name_max_len(shm->race_teams, conf->num_teams); // TODO: replace by config
    max_car_name_len = get_car_name_max_len(&shm->race_cars[0][0], shm->total_num_cars);
    title_length = strlen(RACE_STATISTICS);
    team_id_col_width =  strlen(CAR_TEAM_ID);
    car_laps_col_width =  strlen(CAR_NUM_COMPLETED_LAPS);
    car_box_stops_col_width =  strlen(CAR_NUM_BOX_STOPS);
    max_car_name_col_width =  strlen(CAR_NAME);
    max_team_name_col_width =  strlen(CAR_TEAM_NAME);


    if (max_car_name_len > max_car_name_col_width) {
        max_car_name_col_width = max_car_name_len;
    }

    if (max_team_name_len > max_team_name_col_width) {
        max_team_name_col_width = max_car_name_len;
    }

    row_width = 7 * (NUM_COLS - 1) + MAX_DIGITS + team_id_col_width + car_laps_col_width +
            car_box_stops_col_width + MAX_STATE_LENGTH + max_car_name_col_width + max_team_name_col_width;
    row_sep_half = repeat_str(HORIZONTAL_DELIM, (row_width - title_length) / 2);
    row_sep = repeat_str(HORIZONTAL_DELIM, row_width);

    //SYNC
    shm_cpy = (shared_memory_t *) malloc(sizeof(shared_memory_t));
    memcpy(shm_cpy, shm, sizeof(shared_memory_t));
    //END_SYNC

    race_cars = get_all_cars(shm_cpy);

    selection_sort_cars_by_completed_laps(race_cars, shm_cpy->total_num_cars);

    snprintf(buffer, BUFFER_SIZE,
             "%sRACE STATISTICS%s\n"
             "%*s | %*s | %*s | %*s | %*s | %*s | %*s\n"
             "%s\n",
             row_sep_half, row_sep_half,
             MAX_DIGITS, CAR_ID, -max_car_name_col_width, CAR_NAME,
             -team_id_col_width, CAR_TEAM_ID, -max_team_name_len, CAR_TEAM_NAME,
             car_laps_col_width, CAR_NUM_COMPLETED_LAPS,
             car_box_stops_col_width, CAR_NUM_BOX_STOPS,
             5, CAR_STATE,
             row_sep);

    i = 0;

    while (i < NUM_TOP_CARS + 1) {
        if (i == NUM_TOP_CARS) {
            car = &race_cars[shm_cpy->total_num_cars - 1];
            snprintf(aux, BUFFER_SIZE,"%s\n", row_sep);
            strcat(buffer, aux);
        } else {
            car = &race_cars[0];
        }

        snprintf(aux, BUFFER_SIZE, "  %*d | %*s | %*d | %*s | %*d | %*d | %*s\n",
                 MAX_DIGITS - 2, car->car_id, -max_car_name_col_width,  car->name,
                 -team_id_col_width, car->team->team_id, -max_team_name_col_width,  car->team->team_name,
                 -car_laps_col_width, car->completed_laps, -car_box_stops_col_width,  car->num_box_stops,
                 -MAX_STATE_LENGTH, race_car_state_to_string(car->state));
        strcat(buffer, aux);
        i++;
    }

    snprintf(aux, BUFFER_SIZE, "%s\n", row_sep);
    strcat(buffer, aux);

    snprintf(aux, BUFFER_SIZE, "   %s%d\n   %s%d\n   %s%d\n",
                RACE_NUM_MALFUNCTIONS, shm_cpy->num_malfunctions,
                RACE_NUM_REFUELS, shm_cpy->num_refuels,
                RACE_NUM_CARS_ON_TRACK, shm_cpy->num_cars_on_track);
    strcat(buffer, aux);

    snprintf(aux, BUFFER_SIZE, "%s\n", row_sep);
    strcat(buffer, aux);

    printf("%s", buffer);
}


int main() {
    /**srand(time(NULL));
    int i = 0;
    race_car_t * car_array = (race_car_t *) malloc(10 * sizeof(race_car_t));
    char team_name[15] = {"TEAM SUCCESS"};

    race_team_t team = {0, 0, 0};
    strcpy(team.team_name, team_name);

    while (i < 10) {
        car_array[i].completed_laps = (int) rand() % 10;
        car_array[i].team = &team;
        i++;
    }

    selection_sort_cars_by_completed_laps(car_array, 10);

    i = 0;

    while (i < 10) {
        printf("%s\n", race_car_stats_string(&car_array[i]));
        i++;
    }*/

    srand(time(NULL));
    char buffer[BUFFER_SIZE];
    int i = 0, team_index;
    race_config_t cfg = {1, 800, 5, 1, 3, 40, 8, 1, 25};
    shared_memory_t shmem;
    race_car_t car;
    race_team_t team = {1, 25};
    strcpy(team.team_name, "TEAM SUCCESSSSSSSS");
    shmem.race_teams[0] = team;

    shmem.total_num_cars = 25;
    shmem.num_cars_on_track = 23;
    shmem.num_refuels = 6;
    shmem.num_malfunctions = 7;

    while (i < shmem.total_num_cars) {
        car.team = &team;
        snprintf(buffer, BUFFER_SIZE, "CAR %d", ((int) rand() % 25));
        strcpy(car.name, buffer);
        car.state = (int) rand() % 4;
        car.num_malfunctions = (int) rand() % 4;
        car.num_box_stops = car.num_malfunctions + (int) rand() % 3;
        car.completed_laps = (int) rand() % 7;
        shmem.race_cars[0][i] = car;

        i++;
    }


    init_stats_helper(&cfg, &shmem);
    show_race_stats_table();

}