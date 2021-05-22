/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

//region dependencies

#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include "signal.h"
#include "signal_manager.h"
#include "../stats_helper/stats_helper.h"
#include "../log_generator/log_generator.h"
#include "../../util/global.h"
#include "../../util/file/file.h"
#include "../cmd_validator/cmd_validator.h"

//endregion dependencies

//region private functions prototypes

/**
 * @def segfault_handler
 * @brief Function that handles the response to a segmentation fault signal.
 *
 */
static void segfault_handler();

/**
 * @def sigtstp_handler
 * @brief Function that handles the response to a sigtstp signal.
 *
 */
static void sigtstp_handler();

/**
 * @def sigint_handler
 * @brief Function that handles the response to a sigint signal.
 *
 */
static void sigint_handler();

/**
 * @def sigusr1_handler
 * @brief Function that handles the response to a sigusr1 signal.
 *
 */
static void sigusr1_handler();

/**
 * @def sigint_handler
 * @brief Function that handles the response to a signal by default.
 *
 * @param signum
 * Signal received.
 *
 */
static void default_sig_handler(int signum);

//endregion private functions prototypes

void signal_handler(int signum) {
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

static void segfault_handler() {
    generate_log_entry(SIGNAL_RECEIVE, (void *) SIGNAL_SIGSEGV, NULL);
    printf("[%ul] WELL... THAT ESCALATED QUICKLY...\n", getpid());
}

static void sigtstp_handler() {
    generate_log_entry(SIGNAL_RECEIVE, (void *) SIGNAL_SIGTSTP, NULL);

    if (shm->sync_s.race_running) {
        show_stats_table();
    } else {
        generate_log_entry(RACE_NOT_STARTED, (void *) SIGNAL_IGNORE, NULL);
    }
}

static void sigint_handler() {
    generate_log_entry(SIGNAL_RECEIVE, (void *) SIGNAL_SIGINT, NULL);

    if (!shm->sync_s.race_running) {
        int named_pipe_fd;

        named_pipe_fd = open_file(RACE_SIMULATOR_NAMED_PIPE, O_WRONLY);
        write(named_pipe_fd, (void *) EXIT_SIMULATION, sizeof(EXIT_SIMULATION));
        close_fd(named_pipe_fd);

    } else {
        shm->sync_s.race_interrupted = true;
        shm->sync_s.race_loop = false;
    }
}

static void sigusr1_handler() {
    generate_log_entry(SIGNAL_RECEIVE, (void *) SIGNAL_SIGUSR1, NULL);

    if (shm->sync_s.race_running) {
        shm->sync_s.race_interrupted = true;
        shm->sync_s.race_loop = true;
    } else {
        generate_log_entry(RACE_NOT_STARTED, (void *) SIGNAL_IGNORE, NULL);
    }
}

static void default_sig_handler(int signum) {
    char buffer[SMALLEST_SIZE];
    snprintf(buffer, SMALLEST_SIZE, "%d", signum);
    generate_log_entry(SIGNAL_RECEIVE, (void *) buffer, NULL);
}