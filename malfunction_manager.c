#include "global.h"
#include <stdlib.h>
#include "util/exception_handler.h"

void malfunction_manager(){

    S_DEBUG_MSG(EXITING_PROCESS, MALFUNCTION_MANAGER)

    s_throw_exception_end_exit(NOT_IMPLEMENTED_EXCEPTION, MALFUNCTION_MANAGER);

    exit(EXIT_SUCCESS);
}