#include "global.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "util/error_handler.h"

#define MALFUNCTION_MANAGER "MALFUNCTION MANAGER"

int malfunction_manager(){
    throw_error_end_exit(ERROR_NOT_IMPLEMENTED, MALFUNCTION_MANAGER);
}