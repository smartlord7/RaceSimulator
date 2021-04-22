//
// Created by jfgar on 22/04/2021.
//

#ifndef LOG_GENERATOR_C_PRIORITY_QEUE_H
#define LOG_GENERATOR_C_PRIORITY_QEUE_H

#include "race_car_t.h"
#include "race_box_t.h"

typedef struct queue_element_t queue_element;

struct queue_element_t {
    int priority;
    race_car_t race_car;
}

void insert_car_in_waiting_queue(race_car_t car);

void priority_queue_init(int queue_capacity, int fuel_tank_capacity);

#endif //LOG_GENERATOR_C_PRIORITY_QEUE_H
