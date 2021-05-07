//
// Created by jfgar on 07/05/2021.
//
#include "util/exception_handler/exception_handler.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

#define PIPE_NAME "PIPE_NAME"
#define NAMED "NAMED"

#define true 1
#define false 0

#define BUFFER_SIZE 1024

void open_named_pipe(const char * pipe_name, int * fd, int mode){
    assert(pipe_name != NULL && fd != NULL);

    throw_if_exit((*fd = open(pipe_name, mode)) == -1, NAMED_PIPE_OPEN_EXCEPTION, pipe_name);
}

void read_input(int fd) {
    int n;
    char buffer[BUFFER_SIZE];

    while(1) {
        fgets(buffer, sizeof(buffer), stdin);
        if(buffer == NULL) {
            return;
        }
        if(write(fd, buffer, sizeof(buffer)) < 0) throw_and_exit(PIPE_FAILED_WRITE_EXCEPTION, NAMED);
    }
}
int main() {
    int fd;

    open_named_pipe(PIPE_NAME, &fd, O_WRONLY);

    read_input(fd);

    printf("CLOSING CONNECTION...\n");
    return 0;
}