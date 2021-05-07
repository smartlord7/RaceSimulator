/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "math.h"
#include "../../structs/malfunction/malfunction_t.h"
#include "../../ipcs/message_queue/msg_queue.h"
#include "../../util/numbers/numbers.h"
#include "malfunction_manager.h"

// endregion dependencies

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
    DEBUG_MSG(PROCESS_RUN, MALFUNCTION_MANAGER)

    wait_condition_change();

    uint malfunction_interval = tu_to_usec(config.malfunction_interval);
    int i, j, rdm_index;
    float prob_malfunction;
    race_car_t current_car;
    malfunction_t msg;
    sleep(1); // TODO REMOVE

    while (true) {
        for (i = 0; i < config.num_teams; i++) {
            for (j = 0; j < shm->race_teams[i].num_cars; j++) {
                current_car = shm->race_cars[i][j];

                SYNC
                if (current_car.state == IN_BOX || current_car.state == SAFETY) {
                    END_SYNC
                    continue;
                }
                END_SYNC

                prob_malfunction = 1 - current_car.reliability;

                if (random_uniform_event(prob_malfunction)) {

                    SYNC
                    shm->cars_on_track--;
                    shm->num_malfunctions++;
                    END_SYNC

                    rdm_index = random_int(0, NUM_MALFUNCTIONS - 1);
                    snprintf(msg.malfunction_msg, LARGE_SIZE + MAX_LABEL_SIZE, malfunction_msgs[rdm_index], current_car.car_id);
                    msg.car_id = current_car.car_id;
                    snd_msg(malfunction_msg_q_id, (void *) &msg, sizeof(msg));
                }
            }
        }

        usleep(malfunction_interval);
    }

    DEBUG_MSG(PROCESS_EXIT, MALFUNCTION_MANAGER)

    exit(EXIT_SUCCESS);
}

// endregion public functions