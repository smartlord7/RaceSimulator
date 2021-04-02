#include "global.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "util/error_handler.h"

void malfunction_manager(void * data){
    throw_error(ERROR_NOT_IMPLEMENTED, MALFUNCTION_MANAGER);
}