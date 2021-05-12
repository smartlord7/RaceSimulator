//
// Created by jfgar on 22/04/2021.
//

#include <stdlib.h>
#include <string.h>
#include "priority_queue.h"

#define PRIORITY_ORDER 2
#define MAX_QUEUE_SIZE 100

int safety_state_priority, queue_size;
queue_element_t priority_queue[MAX_QUEUE_SIZE];
queue_element_t root;

void bring_up(int index);

void priority_queue_init(int fuel_tank_capacity){
    safety_state_priority = PRIORITY_ORDER * fuel_tank_capacity;
    bzero(priority_queue, MAX_QUEUE_SIZE * sizeof(queue_element_t));
    queue_size = 0;
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
    queue_element_t element;

    element.race_car = car;
    element.priority = priority;

    return element;
}

void update_root(){
    root = priority_queue[0];
}

void swap(queue_element_t * a, queue_element_t * b){
    queue_element_t temp;

    temp = *a;
    *a = *b;
    *b = temp;

}

int insert_element(race_car_t car) {
    int i;
    queue_element_t node;

    node = create_element(car, calculate_priority(car));

    i = 0;
    while (i < MAX_QUEUE_SIZE){
        if (priority_queue[i].priority == 0){
            priority_queue[i] = node;
            queue_size += 1;
            break;
        } else if (node.priority >= priority_queue[i].priority) {
            i = 2 * i + 2;
        } else if (node.priority < priority_queue[i].priority) {
            i = 2 * i + 1;
        }
    }

    if (i < MAX_QUEUE_SIZE) {
        bring_up(i);
    }

    return EXIT_FAILURE;
}

void bring_up(int index) {
    int i = index;

    while (i != 0 && priority_queue[i].priority > priority_queue[(i - 1) / 2].priority ) {
        swap(&priority_queue[i], &priority_queue[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    update_root();  
}

void update_position(int index){
    int i = index;

    while (true) {
        if (priority_queue[i].priority > priority_queue[2 * i + 2].priority) {
            swap(&priority_queue[i], &priority_queue[2 * i + 2]);
            i = 2 * i + 2;

        } else if (priority_queue[i].priority < priority_queue[2 * i + 1].priority) {
            swap(&priority_queue[i], &priority_queue[2 * i + 1]);
            i = 2 * i + 1;

        } else break;
    }

}

race_car_t get_next_car() {
    race_car_t car;

    car = priority_queue[0].race_car;

    if (queue_size > 1) {
        swap(&priority_queue[0], &priority_queue[queue_size - 1]);

        update_position(0);
        update_root();
    }

    bzero(&priority_queue[queue_size - 1], sizeof(queue_element_t));
    queue_size--;

    return car;
}

/*
int find_car(race_car_t car){
    int i = 0;

    while (i < MAX_QUEUE_SIZE) {

    }
}*/