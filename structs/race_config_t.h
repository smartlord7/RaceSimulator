#ifndef RACESIMULATOR_RACE_CONFIG_H
#define RACESIMULATOR_RACE_CONFIG_H

typedef struct race_config_t race_config_t;

struct race_config_t {
    float time_units_per_sec,
            lap_distance,
            malfunction_interval,
            min_repair_time,
            max_repair_time,
            fuel_tank_capacity;
    int laps_per_race, num_teams, max_cars_per_team;
};

char * race_config_to_string(race_config_t * cfg);

#endif //RACESIMULATOR_RACE_CONFIG_H