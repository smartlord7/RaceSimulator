/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 18/05/2021
*/

// region dependencies

#include "mqueue.h"
#include "assert.h"
#include "unistd.h"
#include "sys/stat.h"
#include "pipe.h"
#include "../../util/exception_handler/exception_handler.h"

// endregion dependencies

// region public functions

void create_unn_pipe(int fds[2]){
    assert(fds != NULL);

    throw_if_exit(pipe(fds) == -1, UNNAMED_PIPE_CREATE_EXCEPTION, NULL);
}

void create_named_pipe(const char * pipe_name){
    assert(pipe_name != NULL);

    unlink(pipe_name);

    throw_if_exit(mkfifo(pipe_name, O_CREAT | O_EXCL | 0600) == -1, NAMED_PIPE_CREATE_EXCEPTION, pipe_name);
}

void destroy_named_pipe(const char * pipe_name) {
    assert(pipe_name != NULL);

    unlink(pipe_name);
}

// endregion public functions