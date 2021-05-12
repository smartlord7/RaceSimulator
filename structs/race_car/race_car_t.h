/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 31/03/2021
*/

#ifndef RACESIMULATOR_C_RACE_CAR_H
#define RACESIMULATOR_C_RACE_CAR_H

// region dependencies

#include "../race_team/race_team_t.h"
#include "../race_car_state_change/race_car_state_change_t.h"
#include "../../ipcs/sync/cond_var/cond_var.h"
#include "../../ipcs/sync/mutex/mutex.h"

// endregion dependencies

typedef struct race_team_t race_team_t;

// region constants

#define true 1
#define false 0
#define RACE_CAR "RACE_CAR"
#define MAX_LABEL_SIZE 50
#define BUF_SIZE 512
#define NULL_STR_SIZE 5
#define SAFETY_CONSUMPTION_RATIO 0.4
#define SAFETY_SPEED_RATIO 0.3

// endregion constants

// region structures

/**
 * @enum race_car_state
 * @brief Enum that represents the allowed states of a race car.
 */
typedef enum race_car_state {
   RACE = 0,
   SAFETY = 1,
   IN_BOX = 2,
   NON_FIT = 3,
   FINISH = 4
} race_car_state;

/**
 * @struct race_car_t
 * @brief Struct that represents a race car.
 *
 * @var race_car_t::team
 * Pointer to the team that owns the race car (acts as a navigation property).
 *
 * @var race_car_t::name
 * The name of the race car.
 *
 * @var race_car_t::state
 * The current state of the race car.
 *
 * @var race_car_t::car_id
 * The id of the race car.
 *
 * @var race_car_t::completed_laps
 * The number of completed laps of the race car.
 *
 * @var race_car_t::num_box_stops
 * The number of times the race car had to stop in a race box due to a malfunction_q_id/refueling purposes.
 *
 * @var race_car_t::num_malfunctions
 * The number of malfunctions the race car had.
 *
 * @var race_car_t::num_of_refuels
 * The number of refuels the race car had.
 *
 * @var race_car_t::remaining_fuel
 * The remaining fuel, in liters, of the race car.
 *
 * @var race_car_t::consumption
 * The consumption in normal conditions, in liters per time unit, of the race car.
 *
 * @var race_car_t::current_consumption
 * The current consumption of the race car, since it can monitor_change according to its current state.
 *
 * @var race_car_t::speed
 * The speed in normal conditions, in meters per time unit, of the race car.
 *
 * @var race_car_t::current_speed
 * The current speed of the race car, since it can monitor_change according to its current state.
 *
 * @var race_car_t::reliability
 * The reliability, between 0 and 1, of the race car.
 *
 * @var race_car_t::current_pos
 * The current position of the race car on the racing track.
 *
 * @var race_car_t::timer
 * The time, in seconds, between the race race_running and when the race car crosses the finish line.
 */
typedef struct race_car_t{
    race_team_t * team;
    char name[MAX_LABEL_SIZE];
    race_car_state state;
    mutex_t cond_mutex, access_mutex;
    cond_t cond;
    int car_id,
            completed_laps,
            num_box_stops,
            num_malfunctions,
            num_of_refuels;
    float remaining_fuel,
            consumption,
            current_consumption,
            speed,
            current_speed,
            reliability,
            current_pos;
} race_car_t;

// endregion structures

// region public functions prototypes
/**
 * @def race_car
 * @brief Function that creates a new race car (constructor)
 *
 * @param team
 * The team which owns the race car.
 *
 * @param car_id
 * The id of the race car.
 *
 * @param consumption
 * The consumption, in liters per time unit, of the race car.
 *
 * @param speed
 * The speed, in meters per time unit, of the race car.
 *
 * @param reliability
 * The reliability, between 0 and 1, of the race car.
 *
 * @param initial_fuel
 * The fuel, in liters, the race car starts with.
 *
 * @return a pointer to the created car.
 *
 * @throws MemoryAllocationException if the malloc call for the race car returns a NULL pointer.
 */
extern race_car_t * race_car(race_team_t * team, char * car_name, int car_id, float consumption, float speed, float reliability, float initial_fuel);

/**
 * @def race_car_to_string
 * @brief Function that retrieves a string representation of a race car.
 *
 * @param race_car
 * the race_car in question.
 *
 * @return a string holding the car information.
 *
 * @throws MemoryAllocationException if the malloc call for the string returns a NULL pointer.
 */
extern char * race_car_to_string(race_car_t * race_car);

/**
 * @def set_state
 * @brief Function that alters the current state of a race car and all its properties
 * correlated with that same state.
 *
 * @param race_car
 * The race car which will have its current state altered.
 *
 * @param state
 * The new state of the race car.
 *
 */
extern void set_state(race_car_t * race_car, race_car_state state);

extern char * race_car_stats_string(race_car_t * car);

// endregion public functions prototypes

#endif //RACESIMULATOR_C_RACE_CAR_H