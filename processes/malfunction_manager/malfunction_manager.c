/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include <unistd.h>
#include "stdio.h"
#include "stdlib.h"
#include "../../structs/malfunction/malfunction_t.h"
#include "../../ipcs/message_queue/msg_queue.h"
#include "../../util/numbers/numbers.h"
#include "malfunction_manager.h"

// endregion dependencies

// region private functions prototypes

static void generate_malfunctions(void);

// endregion private functions prototypes

// region global variables

char malfunction_msgs[NUM_MALFUNCTIONS][LARGE_SIZE] = {
        ENGINE_SPUTTERING_MSG,
        STEERING_WHEEL_SHAKING_MSG,
        TIRES_FLAT_MSG,
        ALTERNATOR_FAILING_MSG,
        RADIATOR_LEAKING_MSG,
        TRANSMISSION_FLUID_LEAKING_MSG,
        LIGHTS_FAILING_MSG,
        EXCESSIVE_OIL_CONSUMPTION_MSG,
        BRAKES_FAILING
};

// endregion global variables

// region public functions

void malfunction_manager(){
    DEBUG_MSG(PROCESS_RUN, ENTRY, MALFUNCTION_MANAGER)

    generate_malfunctions();

    DEBUG_MSG(PROCESS_EXIT, ENTRY, MALFUNCTION_MANAGER)
}

// endregion public functions

// region private functions

static void generate_malfunctions(void) {
    int i, j, rdm_index;
    long int malfunction_interval;
    float prob_malfunction;
    race_car_t * car;
    malfunction_t msg;

    malfunction_interval = tu_to_msec(config.malfunction_interval);

    SYNC
    while (!shm->sync_s.race_running) {
        wait_cond(&shm->sync_s.cond, &shm->sync_s.mutex);
    }
    END_SYNC

    while (true) {
        for (i = 0; i < config.num_teams; i++) {
            for (j = 0; j < shm->race_teams[i].num_cars; j++) {

                car = &shm->race_cars[i][j];

                SYNC_CAR
                if (car->state != RACE) {
                    END_SYNC_CAR
                    continue;
                }

                prob_malfunction = 1 - car->reliability;

                if (random_uniform_event(prob_malfunction)) {
                    shm->num_malfunctions++;

                    rdm_index = random_int(0, NUM_MALFUNCTIONS - 1);
                    snprintf(msg.malfunction_msg, LARGE_SIZE + MAX_LABEL_SIZE, malfunction_msgs[rdm_index], car->car_id);
                    msg.car_id = car->car_id;

                    SYNC
                    if(shm->sync_s.race_running == false) {
                        END_SYNC
                        return;
                    }
                    END_SYNC

                    snd_msg(malfunction_q_id, (void *) &msg, sizeof(msg));
                }
                END_SYNC_CAR
            }
        }

        ms_sleep(malfunction_interval);
    }
}

// endregion private functions