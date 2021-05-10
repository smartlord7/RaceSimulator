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
#define REFUEL_TIME_FACTOR 2

#define TEAM_MANAGER "TEAM_MANAGER"
#define RACE_CAR_SAY "[RACE CAR %d] "
#define TEAM_MANAGER_SAY "[TEAM MANAGER %d] "
#define PIPE_FD "PIPE FD: %d"
#define CAR_MOVE RACE_CAR_SAY "RAN %.2fm!"
#define CAR_FINISH RACE_CAR_SAY "FINISHED THE RACE!"
#define CAR_COMPLETE_LAP RACE_CAR_SAY "COMPLETED %d LAP(S)!"
#define CAR_FIX TEAM_MANAGER_SAY "FIXING CAR %d..."
#define CAR_REFUEL TEAM_MANAGER_SAY "REFUELING CAR %d..."
#define BOX_CAR_ARRIVE TEAM_MANAGER_SAY "THE CAR %d HAS ARRIVED TO THE BOX!"
#define BOX_RESERVE TEAM_MANAGER_SAY "THE BOX HAS BEEN RESERVED!"
#define GLOBAL_CLOCK "[GLOBAL_CLOCK] %d tu"

#define car_close_to_box (car->current_pos == 0 || (car->current_pos <= config.lap_distance && car->current_pos + car->current_speed * config.time_units_per_sec >= config.lap_distance))
#define CHANGE_CAR_STATE(state) \
car_state_change.new_state = state; \
SYNC_CAR \
set_state(car, state); \
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