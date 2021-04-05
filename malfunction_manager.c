/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

// region dependencies

#include <stdlib.h>
#include "exception_handler.h"
#include "global.h"
#include "malfunction_manager.h"

// endregion dependencies

// region public functions

void malfunction_manager(){

    DEBUG_MSG(EXITING_PROCESS, MALFUNCTION_MANAGER)

    throw_exception_and_stay(NOT_IMPLEMENTED_EXCEPTION, MALFUNCTION_MANAGER);

    exit(EXIT_SUCCESS);
}

// endregion public functions