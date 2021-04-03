#ifndef RACESIMULATOR_SHARED_MEMORY_H
#define RACESIMULATOR_SHARED_MEMORY_H

#include "race_config_t.h"
#include "race_car_t.h"
#include "pthread.h"

typedef struct shared_memory shared_memory;

struct shared_memory {
    race_config_t * cfg;
    race_team_t * race_teams;
    race_car_t * race_cars;
    race_box_t * boxes;
    int cars_on_track, total_num_cars;
    pid_t * child_processes_ids;
};

#endif //RACESIMULATOR_SHARED_MEMORY_H