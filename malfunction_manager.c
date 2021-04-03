#include "global.h"
#include "util/debug.h"
#include <stdlib.h>
#include "util/error_handler.h"

void malfunction_manager(void * data){

    DEBUG_MSG(EXITING_PROCESS, MALFUNCTION_MANAGER)

    throw_error_end_exit(ERROR_NOT_IMPLEMENTED, MALFUNCTION_MANAGER);

    exit(EXIT_SUCCESS);
}