/*
 * Authors:
 *  - Joao Filipe Guiomar Artur, 2019217853
 *  - Sancho Amaral Simoes, 2019217590
 *
 * Date of creation: 02/04/2021
 */

#include "global.h"
#include <stdlib.h>
#include "util/exception_handler.h"

void malfunction_manager(){

    DEBUG_MSG(EXITING_PROCESS, MALFUNCTION_MANAGER)

    throw_exception_and_stay(NOT_IMPLEMENTED_EXCEPTION, MALFUNCTION_MANAGER);

    exit(EXIT_SUCCESS);
}