#include <unistd.h>
#include "assert.h"
#include "fcntl.h"
#include "file.h"
#include "../exception_handler/exception_handler.h"

int open_file(const char * file_name, int mode) {
    assert(file_name != NULL);

    int fd = open(file_name, mode);

    throw_if_exit(fd == -1, FILE_OPEN_EXCEPTION, file_name);

    return fd;
}

void close_fd(int fd) {
    assert(fd > 0);

    throw_if_exit(close(fd) == -1, FILE_DESCRIPTOR_CLOSE_EXCEPTION, fd);
}
