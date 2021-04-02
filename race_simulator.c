#include <stdlib.h>
#include "global.h"
#include <unistd.h>
#include "util/process_manager.h"
#include "ipc_manager.h"
#include "race_config_reader.h"
#include "log_generator.h"

#if DEBUG
#include "util/debug.h"
#endif

#define CONFIG_FILE_NAME "config.txt"
#define LOG_FILE_NAME "log.txt"


int main() {
    log_init(LOG_FILE_NAME);
    generate_log_entry(SIMULATION_START, NULL);
    
    race_config_reader_init(CONFIG_FILE_NAME);
    race_config_t * cfg = read_race_config();
    race_config_reader_reset();

    DEBUG_MSG(RACE_CONFIG_CREATED, "")
    show_race_config(cfg);

    create_ipcs();

    create_process(RACE_MANAGER, race_manager, NULL);
    DEBUG_MSG(PROCESS_CREATED, RACE_MANAGER)

    create_process(MALFUNCTION_MANAGER, malfunction_manager, NULL);
    DEBUG_MSG(PROCESS_CREATED, MALFUNCTION_MANAGER)

    destroy_ipcs();
    generate_log_entry(SIMULATION_END, NULL);
    free(cfg);

    return EXIT_SUCCESS;
}

