#include <sys/stat.h>
#include <errno.h>
#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include "util/global.h"
#include "util/exception_handler/exception_handler.h"
#include "util/file/file.h"

#define RACE_SIMULATOR_NAMED_PIPE "RACE_SIMULATOR_NAMED_PIPE"

void read_input(int fd) {
    char buffer[MAX_BUFFER_SIZE];

    while(1) {
        fgets(buffer, sizeof(buffer), stdin);
        if(write(fd, buffer, sizeof(buffer)) < 0) throw_and_exit(PIPE_FAILED_WRITE_EXCEPTION, RACE_SIMULATOR_NAMED_PIPE);
    }
}
int main() {
    int fd;

    throw_if_exit(mkfifo(RACE_SIMULATOR_NAMED_PIPE, O_CREAT | O_EXCL | 0600) == -1 || (errno == EEXIST), NAMED_PIPE_CREATE_EXCEPTION, RACE_SIMULATOR_NAMED_PIPE);
    fd = open_file(RACE_SIMULATOR_NAMED_PIPE, O_WRONLY);
    read_input(fd);

    printf("CLOSING CONNECTION...\n");
    return 0;
}