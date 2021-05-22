/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef STRUCTS_RACE_CAR_STATE_CHANGE_T_H
#define STRUCTS_RACE_CAR_STATE_CHANGE_T_H

//region dependencies

#include "../race_car/race_car_t.h"

//endregion dependencies

//region structures

/**
 * @struct race_car_state_change_t
 * @brief Struct to store info about a race car state change.
 *
 * @var race_car_state_change_t::car_team_index
 * ID of the car within the team.
 *
 * @var race_car_state_change_t::team_id
 * ID of the team who owns the car.
 *
 * @var race_car_state_change_t::prev_state
 * State of the car before the change.
 *
 * @var race_car_state_change_t::new_state
 * State of the car after the change.
 *
 * @var race_car_state_change_t::malfunctioning
 * Code for the malfunction if the change was provoked by a malfunction.
 *
 */
typedef struct race_car_state_change_t {
    int car_team_index, team_id, prev_state, new_state, malfunctioning : 1;
} race_car_state_change_t;

//endregion structures

#endif //STRUCTS_RACE_CAR_STATE_CHANGE_T_H
