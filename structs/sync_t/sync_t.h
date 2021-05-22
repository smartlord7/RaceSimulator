/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef RACESIMULATOR_C_SYNC_T_H
#define RACESIMULATOR_C_SYNC_T_H

//region dependencies

#include <pthread.h>
#include "../../ipcs/sync/mutex/mutex.h"
#include "../../ipcs/sync/cond_var/cond_var.h"

//endregion dependencies

//region structures

typedef struct sync_t sync_t;

typedef struct sync_t {
    int race_running, race_interrupted, clock_active, num_clock_waiters, global_time;
    mutex_t access_mutex, clock_rise_mutex, clock_valley_mutex;
    cond_t cond, clock_rise_cond, clock_valley_cond;
} sync_t;

//endregion structures

#endif //RACESIMULATOR_C_SYNC_T_H
