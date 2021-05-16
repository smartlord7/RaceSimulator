#define _GNU_SOURCE 1

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "memory_mapped_file.h"
#include "../../util/exception_handler/exception_handler.h"

#define true 1
#define false 0
#define MMAP_OFFSET 0

struct stat statbuf;
size_t mmap_size;

char * create_mmap_file(int fd, size_t * size) {
    char * mmap_file;

    if(fstat(fd, &statbuf)) {
        throw_and_exit(FILE_STATS_EXCEPTION, "");
    }

    *size = statbuf.st_size;
    mmap_size = statbuf.st_size;

    if((mmap_file = mmap(NULL, statbuf.st_size, PROT_WRITE , MAP_SHARED, fd, 0)) == (caddr_t) - 1) {
        throw_and_exit(MMAP_CREATE_EXCEPTION,"");
    }

    return mmap_file;
}

int write_mmap(char * mmap_addr, char * buffer, int fd, size_t * file_size) {
    size_t old_size = *file_size;
    size_t old_mmap_size = mmap_size;

    if(*file_size + strlen(buffer) >= mmap_size) {
        mmap_size *= 2;
        if (ftruncate(fd, (off_t) mmap_size) != 0) {
            throw_and_exit(FILE_TRUNC_EXCEPTION, NULL);
        }

        if ((mmap_addr = mremap(mmap_addr, old_mmap_size, mmap_size, MREMAP_MAYMOVE)) == MAP_FAILED) {
            throw_and_exit(MMAP_REMAP_EXCEPTION, NULL);
        }
    }
    *file_size += strlen(buffer);

    memcpy(mmap_addr + old_size, buffer, *file_size - old_size);

    return EXIT_SUCCESS;
}

int destroy_mmap(char * mmap, int fd, size_t file_size) {

    if(munmap(mmap, file_size) < 0) {
        throw_and_exit(MMAP_DESTROY_EXCEPTION, NULL);
    }

    if (ftruncate(fd, (off_t) file_size) != 0) {
        throw_and_exit(FILE_TRUNC_EXCEPTION, NULL);
    }

    return EXIT_SUCCESS;
}