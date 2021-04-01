//
// Created by jfgar on 01/04/2021.
//

#define CAR_RACE_STATUS 0
#define CAR_SAFETY_STATUS 1
#define CAR_BOX_STATUS 2
#define CAR_NON_FIT_STATUS 3
#define CAR_FINISHED_STATUS 4

#define BOX_FREE_STATUS 0
#define BOX_RESERVED_STATUS 1
#define BOX_OCCUPIED_STATUS 2

#ifndef MAIN_C_STRUCTS_H
#define MAIN_C_STRUCTS_H

#endif //MAIN_C_STRUCTS_H

typedef struct shared_memory shared_memory;
typedef struct race_car race_car;
typedef struct car_status car_status;
typedef struct race_team race_team;
typedef struct box_status box_status;
typedef struct box box;
typedef struct race_team race_team;

struct shared_memory {
    //race_config configs;
    car_status * racing_cars_status;
    box_status * team_boxes_status;
    int num_of_cars_on_track;

};


struct race_car{
    int team_id, car_id, completed_laps;
    int num_of_box_stops, num_of_malfunctions, num_of_refuels;
    float fuel_tank_capacity, fuel_tank, consumption, speed;
};


struct car_status {
    int car_id, team_id, status;
};


struct race_team {
    int team_id, box_id, num_of_cars;
    int * car_ids;
    race_car * cars;
};

struct box{
    int team_id;
    race_car current_car;
};

struct box_status {
    int team_id, status;
};
