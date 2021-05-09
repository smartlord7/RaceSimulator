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
#include "../../structs/malfunction/malfunction_t.h"
#include "../../ipcs/message_queue/msg_queue.h"
#include "../../util/numbers/numbers.h"
#include "malfunction_manager.h"
#include "../../util/exception_handler/exception_handler.h"

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
    DEBUG_MSG(PROCESS_RUN, DEBUG_LEVEL_ENTRY, MALFUNCTION_MANAGER)

    generate_malfunctions();

    DEBUG_MSG(PROCESS_EXIT, DEBUG_LEVEL_ENTRY, MALFUNCTION_MANAGER)

    exit(EXIT_SUCCESS);
}

// endregion public functions

// region private functions

static void generate_malfunctions(void) {
    init_cond(&shm->sync_s.start_cond, true);
    init_mutex(&shm->sync_s.mutex, true);

    int i, j, rdm_index;
    long int malfunction_interval;
    float prob_malfunction;
    race_car_t current_car;
    malfunction_t msg;

    malfunction_interval = tu_to_msec(config.malfunction_interval);

    lock_mutex(&shm->sync_s.mutex);
    while (!shm->sync_s.race_start) {
        wait_cond(&shm->sync_s.start_cond, &shm->sync_s.mutex);
    }
    unlock_mutex(&shm->sync_s.mutex);

    while (true) {
        for (i = 0; i < config.num_teams; i++) {
            for (j = 0; j < shm->race_teams[i].num_cars; j++) {
                current_car = shm->race_cars[i][j];
                if (current_car.state == IN_BOX || current_car.state == SAFETY) {
                    continue;
                }

                prob_malfunction = 1 - current_car.reliability;

                if (random_uniform_event(prob_malfunction)) {
                    shm->cars_on_track--;
                    shm->num_malfunctions++;

                    rdm_index = random_int(0, NUM_MALFUNCTIONS - 1);
                    snprintf(msg.malfunction_msg, LARGE_SIZE + MAX_LABEL_SIZE, malfunction_msgs[rdm_index], current_car.car_id);
                    msg.car_id = current_car.car_id;
                    snd_msg(malfunction_q_id, (void *) &msg, sizeof(msg));
                }
            }
        }

        ms_sleep(malfunction_interval);
    }
}

// endregion private functions