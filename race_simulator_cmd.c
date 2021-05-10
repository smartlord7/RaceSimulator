#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include "util/global.h"
#include "util/exception_handler/exception_handler.h"
#include "util/file/file.h"

#define NAMED "NAMED"

void read_input(int fd) {
    char buffer[LARGE_SIZE];

    while(1) {
        fgets(buffer, sizeof(buffer), stdin);
        if(write(fd, buffer, sizeof(buffer)) < 0) throw_and_exit(PIPE_FAILED_WRITE_EXCEPTION, NAMED);
    }
}
int main() {
    int fd;

    fd = open_file(RACE_SIMULATOR_NAMED_PIPE, O_WRONLY);
    read_input(fd);

    printf("CLOSING CONNECTION...\n");
    return 0;
}