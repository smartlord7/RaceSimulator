//
// Created by jfgar on 22/04/2021.
//

#include "priority_qeue.h"

#define PRIORITY_ORDER 2

int safety_state_priority;
int capacity;
int queue[3];

void priority_queue_init(int queue_capacity, int fuel_tank_capacity){
    capacity = queue_capacity;
    safety_state_priority = PRIORITY_ORDER * fuel_tank_capacity;
}

int calculate_priority(race_car_t race_car){
    switch (race_car.state) {

        case RACE:
            return safety_state_priority - (int) race_car.remaining_fuel;
        case SAFETY:
            return 2 * safety_state_priority - (int) race_car.remaining_fuel;
        default:
            return -1;
    }
}

struct queue_element_t create_element(race_car_t car, int priority){
    queue_element element;

    element.race_car = car;
    element.priority = priority;

    return element;
}

void insert_car_in_waiting_queue(race_car_t car){
    queue_element race_car;
    int priority;

    priority = calculate_priority(car);
    race_car = create_element(car, priority);



}
