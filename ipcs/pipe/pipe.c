#include "assert.h"
#include "errno.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "pipe.h"
#include "../../util/exception_handler/exception_handler.h"

void create_unn_pipe(int fds[2]){
    assert(fds != NULL);

    throw_if_exit(pipe(fds) != -1, UNNAMED_PIPE_CREATE_EXCEPTION, NULL);
}

void close_fds(const char * file_type, const int * fds, int num_fds) {
    assert(file_type != NULL && fds != NULL && num_fds > 0);
    int i = 0;

    while (i < num_fds) {
        throw_if_exit(close(fds[i]) == -1, FILE_DESCRIPTOR_CLOSE_EXCEPTION, fds[i], file_type);

        i++;
    }
}

void create_named_pipe(const char * pipe_name, int * fd, int mode){
    assert(pipe_name != NULL && fd != NULL);

    unlink(pipe_name);

    throw_if_exit(mkfifo(pipe_name, O_CREAT | O_EXCL | 0600) == -1 || (errno == EEXIST), NAMED_PIPE_CREATE_EXCEPTION, pipe_name);

    throw_if_exit((* fd = open(pipe_name, mode)) == -1, NAMED_PIPE_OPEN_EXCEPTION, pipe_name);
}