#include "unistd.h"
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

void read_stream(int fd, void * buffer, size_t size) {
    assert(fd > 0 && buffer != NULL);

    size_t total = 0;
    ssize_t n;

    while (total < size) {
        n = read(fd, (char * ) buffer + total, size - total);

        throw_if_exit(n == -1, STREAM_READ_EXCEPTION, fd);
        total += n;
    }
}

void write_stream(int fd, void * buffer, size_t size) {
    assert(fd > 0 && buffer != NULL);

    throw_if_exit(write(fd, buffer, size) == -1, STREAM_WRITE_EXCEPTION, fd);
}