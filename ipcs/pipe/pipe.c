#include "mqueue.h"
#include "assert.h"
#include "errno.h"
#include "unistd.h"
#include "sys/stat.h"
#include "pipe.h"
#include "../../util/exception_handler/exception_handler.h"

void create_unn_pipe(int fds[2]){
    assert(fds != NULL);

    throw_if_exit(pipe(fds) != -1, UNNAMED_PIPE_CREATE_EXCEPTION, NULL);
}

int create_named_pipe(const char * pipe_name, int mode){
    assert(pipe_name != NULL);

    int fd;

    unlink(pipe_name);

    throw_if_exit(mkfifo(pipe_name, O_CREAT | O_EXCL | 0600) == -1 || (errno == EEXIST), NAMED_PIPE_CREATE_EXCEPTION, pipe_name);
    sleep(1);

    return fd;
}