#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include "util/global.h"
#include "util/exception_handler/exception_handler.h"
#include "util/file/file.h"

#define RACE_SIMULATOR_NAMED_PIPE "RACE_SIMULATOR_NAMED_PIPE"

int main() {
    int fd;
    char buffer[LARGE_SIZE];

    fd = open_file(RACE_SIMULATOR_NAMED_PIPE, O_WRONLY);

    while (fgets(buffer, LARGE_SIZE, stdin) != NULL) {
        if (write(fd, buffer, sizeof(buffer)) < 0) throw_and_exit(PIPE_FAILED_WRITE_EXCEPTION, RACE_SIMULATOR_NAMED_PIPE);
    }

    printf("CLOSING CONNECTION...\n");
    return 0;
}