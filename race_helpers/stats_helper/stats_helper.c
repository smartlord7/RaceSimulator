#include <assert.h>
#include "malloc.h"
#include "stdlib.h"
#include "string.h"
#include "stats_helper.h"
#include "../../util/strings/strings.h"

race_config_t * conf = NULL;
shared_memory_t * sha_mem = NULL;
mutex_t * mutex = NULL;

static void swap_car(race_car_t * car1, race_car_t * car2);
static void bubble_sort_race_cars(race_car_t * race_cars, int size);
static int get_team_name_max_len();
static int get_car_name_max_len();
static race_car_t * get_all_cars(shared_memory_t * shm_cpy);

void stats_helper_init(race_config_t * cfg, shared_memory_t * shmem, mutex_t * mtx) {
    assert(cfg != NULL && shmem != NULL && mtx != NULL);

    conf = cfg;
    sha_mem = shmem;
    mutex = mtx;
}

void show_stats_table() { // TODO: validate functions result
    char buffer[BUFFER_SIZE], aux[BUFFER_SIZE], * row_sep_half, * row_sep, * race_car_state_str;
    shared_memory_t * shm_cpy = NULL;
    race_car_t * race_cars = NULL, * car = NULL;
    int i, pos, num_table_cars, max_team_name_len, max_car_name_len,
            max_car_name_col_width, max_team_name_col_width, row_width, title_length,
            team_id_col_width, car_laps_col_width, car_box_stops_col_width;

    max_team_name_len = get_team_name_max_len(sha_mem->race_teams, conf->num_teams); // TODO: replace by config
    max_car_name_len = get_car_name_max_len(&sha_mem->race_cars[0][0], sha_mem->total_num_cars);
    num_table_cars = NUM_TOP_CARS + 1;
    title_length = strlen(RACE_STATISTICS);
    team_id_col_width =  strlen(CAR_TEAM_ID);
    car_laps_col_width =  strlen(CAR_NUM_COMPLETED_LAPS);
    car_box_stops_col_width =  strlen(CAR_NUM_BOX_STOPS);
    max_car_name_col_width =  strlen(CAR_NAME);
    max_team_name_col_width =  strlen(CAR_TEAM_NAME);

    if (sha_mem->total_num_cars < num_table_cars) {
        num_table_cars = sha_mem->total_num_cars;
    }

    if (max_car_name_len > max_car_name_col_width) {
        max_car_name_col_width = max_car_name_len;
    }

    if (max_team_name_len > max_team_name_col_width) {
        max_team_name_col_width = max_team_name_len;
    }

    row_width = 7 * (NUM_COLS - 1) + MAX_DIGITS + max_car_name_col_width +
                team_id_col_width + max_team_name_col_width + car_laps_col_width +
                car_box_stops_col_width + MAX_STATE_LENGTH ;
    row_sep = repeat_str(HORIZONTAL_DELIM, row_width);
    row_sep_half = repeat_str(HORIZONTAL_DELIM, (row_width - title_length) / 2);
    shm_cpy = (shared_memory_t *) malloc(sizeof(shared_memory_t));

    lock_mutex(mutex);
    memcpy(shm_cpy, sha_mem, sizeof(shared_memory_t));
    unlock_mutex(mutex);

    race_cars = get_all_cars(shm_cpy);

    bubble_sort_race_cars(race_cars, shm_cpy->total_num_cars); // TODO: give last place to disqualified cars

    snprintf(buffer, BUFFER_SIZE,
             "\n\n%sRACE STATISTICS%s\n"
             " %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s\n"
             "%s\n",
             row_sep_half, row_sep_half,
             -MAX_DIGITS, CAR_RACE_POS,
             -MAX_DIGITS, CAR_ID, -max_car_name_col_width, CAR_NAME,
             -team_id_col_width, CAR_TEAM_ID, -max_team_name_col_width, CAR_TEAM_NAME,
             car_laps_col_width, CAR_NUM_COMPLETED_LAPS,
             car_box_stops_col_width, CAR_NUM_BOX_STOPS,
             5, CAR_STATE,
             row_sep);

    i = 0;

    while (i < num_table_cars) {
        if (i == num_table_cars - 1) {
            pos = sha_mem->total_num_cars;
            car = &race_cars[sha_mem->total_num_cars - 1];
            snprintf(aux, BUFFER_SIZE,"%s\n", row_sep);
            strcat(buffer, aux);
        } else {
            pos = i + 1;
            car = &race_cars[i];
        }

        race_car_state_str = race_car_state_to_string(car->state);

        snprintf(aux, BUFFER_SIZE, " %*d | %*d | %*s | %*d | %*s | %*d | %*d | %*s\n",
                 -MAX_DIGITS, pos,
                 -MAX_DIGITS, car->car_id, -max_car_name_col_width,  car->name,
                 -team_id_col_width, car->team->team_id, -max_team_name_col_width,  car->team->team_name,
                 -car_laps_col_width, car->completed_laps, -car_box_stops_col_width,  car->num_box_stops,
                 -MAX_STATE_LENGTH, race_car_state_str);
        strcat(buffer, aux);
        i++;

        free(race_car_state_str);
    }

    snprintf(aux, BUFFER_SIZE, "%s\n", row_sep);
    strcat(buffer, aux);

    snprintf(aux, BUFFER_SIZE, "   %s%d\n   %s%d\n   %s%d\n",
             RACE_NUM_MALFUNCTIONS, shm_cpy->num_malfunctions,
             RACE_NUM_REFUELS, shm_cpy->num_refuels,
             RACE_NUM_CARS_ON_TRACK, shm_cpy->num_cars_on_track);
    strcat(buffer, aux);

    snprintf(aux, BUFFER_SIZE, "%s\n\n", row_sep);
    strcat(buffer, aux);

    printf("%s\n", buffer);

    free(row_sep_half);
    free(row_sep);
    free(shm_cpy);
    free(race_cars);
}

static void swap_car(race_car_t * car1, race_car_t * car2) {
    race_car_t temp = * car1;
    * car1 = * car2;
    * car2 = temp;
}

static void bubble_sort_race_cars(race_car_t * race_cars, int size) {
    int i, j;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (race_cars[j].completed_laps < race_cars[j + 1].completed_laps) {
                swap_car(&race_cars[j], &race_cars[j + 1]);
            } else if (race_cars[j].completed_laps == race_cars[j + 1].completed_laps) {
                if (race_cars[j].current_pos < race_cars[j].current_pos) {
                    swap_car(&race_cars[j], &race_cars[j + 1]);
                }
            }
        }
    }
}

static int get_team_name_max_len() {
    int max_len = -1, i = 0, len;

    while (i < conf->num_teams) {
        len = (int) strlen(sha_mem->race_teams[i].team_name);
        if (len > max_len) {
            max_len = len;
        }

        i++;
    }

    return max_len;
}

static int get_car_name_max_len() {
    int max_len, len, i, j;
    race_team_t * team = NULL;
    race_car_t * car = NULL;

    max_len = -1;
    i = 0;

    while (i < conf->num_teams) {
        team = &sha_mem->race_teams[i];
        j = 0;

        while (j < team->num_cars) {
            car = &sha_mem->race_cars[i][j];
            len = (int) strlen(car->name);

            if (len > max_len) {
                max_len = len;
            }

            j++;
        }

        i++;
    }


    return max_len;
}

static race_car_t * get_all_cars(shared_memory_t * shm_cpy) {
    int i, j, k;
    race_car_t * cars = NULL;
    race_team_t * team = NULL;

    cars = (race_car_t *) malloc(shm_cpy->total_num_cars * sizeof(race_car_t));
    i = 0;
    k = 0;

    while (i < conf->num_teams) { //TODO: replace by config
        j = 0;
        team = &sha_mem->race_teams[i];

        while (j < team->num_cars) {
            cars[k++] = sha_mem->race_cars[i][j++];
        }

        i++;
    }

    return cars;
}

/**int main() {
    srand(time(NULL));
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
    }

    srand(time(NULL));
    char buffer[BUFFER_SIZE];
    int i = 0, team_index;
    race_config_t cfg = {1, 800, 5, 1, 3, 40, 8, 1, 25};
    shared_memory_t shmem;
    race_car_t car;
    mutex_t mtx;
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
        car.car_id = (int) rand() % 25;
        car.state = (int) rand() % 4;
        car.num_malfunctions = (int) rand() % 4;
        car.num_box_stops = car.num_malfunctions + (int) rand() % 3;
        car.completed_laps = (int) rand() % 29 + 1;
        shmem.race_cars[0][i] = car;

        i++;
    }

    stats_helper_init(&cfg, &shmem, &mtx);
    show_stats_table();

}*/