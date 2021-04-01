#include <stdlib.h>
#include "global.h"
#include "util/error_handler.h"
#include "race_config_reader.h"
#include "ipc_manager.h"

#define CONFIG_FILE_NAME "config.txt"

#if DEBUG
#include "util/debug.h"
#define DEBUG_MSG(msg, ...) if (DEBUG) debug_msg(__FILE__, __LINE__, msg, __VA_ARGS__);
#endif

int main(int argc, char * argv[]) {

    race_config_reader_init(CONFIG_FILE_NAME);
    race_config * cfg = read_race_config();
    race_config_reader_reset();

    DEBUG_MSG(RACE_CONFIG_CREATED, "")
    show_race_config(cfg);

    create_ipcs();

    destroy_ipcs();

    free(cfg);
}
