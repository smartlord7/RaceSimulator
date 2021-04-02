#include <stdlib.h>
#include "global.h"
#include "util/error_handler.h"
#include "race_config_reader.h"
#include "ipc_manager.h"
#include <unistd.h>

#define CONFIG_FILE_NAME "config.txt"
#define RACE_MANAGER "RACE MANAGER"
#define MALFUNCTION_MANAGER "MALFUNCTION MANAGER"

#if DEBUG
#include "util/debug.h"
#define DEBUG_MSG(msg, ...) if (DEBUG) debug_msg(__FILE__, __LINE__, msg, __VA_ARGS__);
#endif

int start_race_manager();
int start_malfunction_manager();

int main(int argc, char * argv[]) {

    race_config_reader_init(CONFIG_FILE_NAME);
    race_config * cfg = read_race_config();
    race_config_reader_reset();

    DEBUG_MSG(RACE_CONFIG_CREATED, "")
    show_race_config(cfg);

    create_ipcs();

    if(start_race_manager() == EXIT_FAILURE){
        throw_error(FATAL_ERROR_CREATE_PROCESS, RACE_MANAGER);
        return EXIT_FAILURE;
    }

    if(start_malfunction_manager() == EXIT_FAILURE){
        throw_error(FATAL_ERROR_CREATE_PROCESS, MALFUNCTION_MANAGER);
        return EXIT_FAILURE;
    }


    destroy_ipcs();

    free(cfg);
    return EXIT_SUCCESS;
}

int start_race_manager(){
    pid_t child;

    child = fork();
    if(child < 0) return EXIT_FAILURE;
    else if(child == 0){
        race_manager();
        exit(EXIT_SUCCESS);
    }
    return EXIT_SUCCESS;
}

int start_malfunction_manager(){
    pid_t child;

    child = fork();
    if(child < 0) return EXIT_FAILURE;
    else if(child == 0){
        malfunction_manager();
        exit(EXIT_SUCCESS);
    }
    return EXIT_SUCCESS;
}
