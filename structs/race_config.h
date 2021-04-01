#ifndef RACESIMULATOR_RACE_CONFIG_H
#define RACESIMULATOR_RACE_CONFIG_H

typedef struct race_config race_config ;

struct race_config {
    float time_units_per_sec,
            lap_distance,
            malfunction_interval,
            min_repair_time,
            max_repair_time,
            fuel_tank_capacity;
    int laps_per_race, num_teams, max_cars_per_team;
};

#endif //RACESIMULATOR_RACE_CONFIG_H
