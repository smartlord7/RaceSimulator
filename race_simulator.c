#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "global.h"
#include "util/process_manager.h"
#include "util/debug.h"
#include "ipc_manager.h"
#include "race_config_reader.h"
#include "log_generator.h"

#define CONFIG_FILE_NAME "config.txt"
#define LOG_FILE_NAME "log.txt"

void free_all() {
    free(mem_struct->cfg);
}

int main() {
    race_config_reader_init(CONFIG_FILE_NAME);
    race_config_t * cfg = read_race_config();

    DEBUG_MSG(RACE_CONFIG_CREATED, "")

    #if DEBUG
    printf("%s", race_config_to_string(cfg));
    #endif

    create_ipcs(cfg->num_teams);

    DEBUG_MSG(IPCS_CREATED, "")

    mem_struct->cfg = cfg; // There are no other application processes, no MUTEX needed.

    log_init(LOG_FILE_NAME);
    generate_log_entry(I_SIMULATION_START, NULL);

    create_process(RACE_MANAGER, race_manager, NULL);

    create_process(MALFUNCTION_MANAGER, malfunction_manager, NULL);

    wait_all();
    destroy_ipcs(cfg->num_teams);

    generate_log_entry(I_SIMULATION_END, NULL);

    return EXIT_SUCCESS;
}

