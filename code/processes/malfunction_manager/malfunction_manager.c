/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include "signal.h"
#include "unistd.h"
#include "stdio.h"
#include "../../structs/malfunction/malfunction_t.h"
#include "../../ipcs/message_queue/msg_queue.h"
#include "../../util/numbers/numbers.h"
#include "malfunction_manager.h"

// endregion dependencies

// region private functions prototypes

/**
 * @def generate_malfunctions
 * @brief Function that randomly generates malfunctions for the race cars.
 */
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
    DEBUG_MSG(PROCESS_RUN, ENTRY, MALFUNCTION_MANAGER, getpid())

    // ignore all signals captured by the race simulator.
    signal(SIGSEGV, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    generate_malfunctions();

    DEBUG_MSG(PROCESS_EXIT, ENTRY, MALFUNCTION_MANAGER, getpid())
}

// endregion public functions

// region private functions

static void generate_malfunctions(void) {
    int i, j, rdm_index;
    float prob_malfunction;
    race_car_t * car;
    malfunction_t msg;

    // wait for the beginning of the race.
    if (!wait_race_start()) {
        return;
    }

    while (true) {
        for (i = 0; i < config.num_teams; i++) {
            for (j = 0; j < shm->race_teams[i].num_cars; j++) {
                car = &shm->race_cars[i][j];

                //check if the car can receive a malfunction
                SYNC_CAR
                if (car->state != RACE) {
                    END_SYNC_CAR
                    continue;
                }

                prob_malfunction = 1 - car->reliability;

                // generate a random malfunction and notify the car affected by it.
                if (random_uniform_event(prob_malfunction)) {
                    rdm_index = random_int(0, NUM_MALFUNCTIONS - 1);
                    snprintf(msg.description, LARGE_SIZE + MAX_LABEL_SIZE, malfunction_msgs[rdm_index], car->name);
                    msg.car_id = car->car_id;
                    snd_msg(malfunction_q_id, (void *) &msg, sizeof(malfunction_t));
                }
                END_SYNC_CAR
            }
        }

        // sleep until next clock rise.
        sync_sleep(config.malfunction_interval);

        if (shm->state == FINISHED || shm->state == CLOSED) {
        // verify if the race has ended or if it may repeat.
            if (shm->hold_on_end) {
                if (!wait_race_start()) {
                    return;
                }
            } else {
                return;
            }
        }
    }
}

// endregion private functions