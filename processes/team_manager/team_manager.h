/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

#ifndef RACESIMULATOR_C_TEAM_MANAGER_H
#define RACESIMULATOR_C_TEAM_MANAGER_H

// region constants

#define REFUEL_MIN_LAPS1 4
#define REFUEL_MIN_LAPS2 2
#define REFUEL_TIME 2

#define TEAM_MANAGER "TEAM_MANAGER"
#define TEAM_BOX "TEAM_BOX"
#define RACE_CAR_SAY "[RACE CAR %s] "
#define TEAM_MANAGER_SAY "[TEAM MANAGER %s] "
#define CAR_MOVE RACE_CAR_SAY "RAN %.2fm!"
#define BOX_CAR_ARRIVE TEAM_MANAGER_SAY "THE CAR %s HAS ARRIVED TO THE BOX!"
#define BOX_RESERVE TEAM_MANAGER_SAY "THE BOX HAS BEEN RESERVED!"

#define car_close_to_box (car->current_pos == 0 || (car->current_pos <= config.lap_distance && car->current_pos + car->current_speed * config.time_units_per_sec >= config.lap_distance))
#define CHANGE_CAR_STATE(new_car_state) \
car_state_change.prev_state = car->state; \
car_state_change.new_state = new_car_state; \
car_state_change.team_id = car->team->team_id; \
if (car_state_change.malfunctioning) { \
generate_log_entry(CAR_MALFUNCTION, (void *) car, (void *) &malfunction.description); \
} \
SYNC_CAR \
set_state(car, new_car_state); \
END_SYNC_CAR \
lock_mutex(&team->pipe_mutex);  \
write_stream(unn_pipe_fds[1], &car_state_change, sizeof(race_car_state_change_t));\
unlock_mutex(&team->pipe_mutex)

// endregion constants

// region public functions

/**
 * @def team_manager
 * @brief Worker function executed when the team manager process is created.
 *
 */
void team_manager();

// endregion public functions

#endif //RACESIMULATOR_C_TEAM_MANAGER_H