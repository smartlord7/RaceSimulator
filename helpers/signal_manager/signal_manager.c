/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

// region dependencies

#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include "signal.h"
#include "../stats_helper/stats_helper.h"
#include "../log_generator/log_generator.h"
#include "../cmd_validator/cmd_validator.h"
#include "../../util/global.h"
#include "../../util/file/file.h"
#include "signal_manager.h"

// endregion dependencies

// region private functions prototypes

/**
 * @def segfault_handler
 * @brief Function that handles the SIGSEGV signal.
 *
 */
static void segfault_handler();

/**
 * @def sigtstp_handler
 * @brief Function that handles the SIGTSTP signal.
 *
 */
static void sigtstp_handler();

/**
 * @def sigint_handler
 * @brief Function that handles the SIGINT signal.
 *
 */
static void sigint_handler();

/**
 * @def sigusr1_handler
 * @brief Function that handles the SIGUSR1 signal.
 *
 */
static void sigusr1_handler();

/**
 * @def sigint_handler
 * @brief Function that handles the response to a signal by default.
 *
 * @param signum
 * The code of the received signal.
 *
 */
static void default_sig_handler(int signum);

// endregion private functions prototypes

// region public functions

void signal_handler(int signum) {

    // filter the given signal code, assigning the associated handler.
    switch (signum) {
        case SIGSEGV:
            segfault_handler();
            break;
        case SIGTSTP:
            sigtstp_handler();
            break;
        case SIGINT:
            sigint_handler();
            break;
        case SIGUSR1:
            sigusr1_handler();
            break;
        default:
            default_sig_handler(signum);
    }
}

// endregion public functions

// region private functions

static void segfault_handler() {
    generate_log_entry(SIGNAL_RECEIVE, (void *) SIGNAL_SIGSEGV, NULL);
    printf("[%ul] WELL... THAT ESCALATED QUICKLY...\n", getpid()); // Default message :-).
    exit(EXIT_FAILURE);
}

static void sigtstp_handler() {
    generate_log_entry(SIGNAL_RECEIVE, (void *) SIGNAL_SIGTSTP, NULL);

    if (shm->state == RUNNING || shm->state == INTERRUPTED) { // if the race is still running or it has been interrupted by a SIGINT or SIGUSR1 (but actually, it is still running till all cars cross the finish line)
        show_stats_table();
    } else { // ignore the signal since the race isn't running.
        generate_log_entry(RACE_NOT_STARTED, (void *) SIGNAL_IGNORE, NULL);
    }
}

static void sigint_handler() {
    generate_log_entry(SIGNAL_RECEIVE, (void *) SIGNAL_SIGINT, NULL);

    if (shm->state == NOT_STARTED) { // allow the user to cancel the simulation before the race starts. Alternative to writing EXIT into the named pipe.
        int named_pipe_fd;

        named_pipe_fd = open_file(RACE_SIMULATOR_NAMED_PIPE, O_WRONLY); // signal the race manager that the simulation is to be ended, via the named pipe.
        write(named_pipe_fd, (void *) EXIT_SIMULATION, sizeof(EXIT_SIMULATION));
        close_fd(named_pipe_fd);

    } else { // if the race is running, then the simulation should end when all cars cross the finish line.
        shm->hold_on_end = false;
        shm->state = INTERRUPTED; // if a SIGUSR1 has been previously signalled, the program will proceed to its overriding.
        notify_race_state_change(); // notify all entities waiting for the correspondent condition to change that the race has been interrupted.
    }
}

static void sigusr1_handler() {
    generate_log_entry(SIGNAL_RECEIVE, (void *) SIGNAL_SIGUSR1, NULL);

    if ((shm->state == RUNNING || shm->state == INTERRUPTED) && !shm->hold_on_end) { // if the race is running or was interrupted by a SIGINT.
        shm->hold_on_end = true; // at the end of the race, the program will wait for the user feedback: start the race again or simply close the simulator.
        shm->state = INTERRUPTED; // if a SIGINT has been previously signalled, the program will proceed to its overriding. This is useful if you, for example, press Ctrl + z but then regret of it :-).
        notify_race_state_change(); // notify all entities waiting for the correspondent condition to change that the race has been interrupted.
    } else {
        generate_log_entry(RACE_NOT_STARTED, (void *) SIGNAL_IGNORE, NULL);
    }
}

static void default_sig_handler(int signum) {
    char buffer[SMALLEST_SIZE]; // if other signal than SIGINT, SIGSEGV, SIGTSTP, SIGUSR1 is captured, the program will still notice it.
    snprintf(buffer, SMALLEST_SIZE, "%d", signum);
    generate_log_entry(SIGNAL_RECEIVE, (void *) buffer, NULL);
}

// endregion private functions