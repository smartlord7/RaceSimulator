#ifndef RACESIMULATOR_SHARED_MEMORY_H
#define RACESIMULATOR_SHARED_MEMORY_H

#include "race_config.h"
#include "race_car.h"

struct shared_memory {
    race_config * cfg;
    race_car * race_cars;
    race_box * boxes;
    int num_active_cars;
};

#endif //RACESIMULATOR_SHARED_MEMORY_H
