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

static void segfault_handler();
static void sigtstp_handler();
static void sigint_handler();
static void sigusr1_handler();
static void default_sig_handler(int signum);

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

    if (shm->state == STARTED) {
        show_stats_table();
    } else {
        generate_log_entry(RACE_NOT_STARTED, (void *) SIGNAL_IGNORE, NULL);
    }
}

static void sigint_handler() {
    generate_log_entry(SIGNAL_RECEIVE, (void *) SIGNAL_SIGINT, NULL);

    if (shm->state == NOT_STARTED) {
        int named_pipe_fd;

        named_pipe_fd = open_file(RACE_SIMULATOR_NAMED_PIPE, O_WRONLY);
        write(named_pipe_fd, (void *) EXIT_SIMULATION, sizeof(EXIT_SIMULATION));
        close_fd(named_pipe_fd);

    } else {
        shm->state = INTERRUPTED;
        shm->hold_on_end = false;
    }
}

static void sigusr1_handler() {
    generate_log_entry(SIGNAL_RECEIVE, (void *) SIGNAL_SIGUSR1, NULL);

    if (shm->state == STARTED) {
        shm->state = INTERRUPTED;
        shm->hold_on_end = true;
    } else {
        generate_log_entry(RACE_NOT_STARTED, (void *) SIGNAL_IGNORE, NULL);
    }
}

static void default_sig_handler(int signum) {
    char buffer[SMALLEST_SIZE];
    snprintf(buffer, SMALLEST_SIZE, "%d", signum);
    generate_log_entry(SIGNAL_RECEIVE, (void *) buffer, NULL);
}
