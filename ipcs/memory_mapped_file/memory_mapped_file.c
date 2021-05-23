/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#define _GNU_SOURCE 1

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "memory_mapped_file.h"
#include "../../util/exception_handler/exception_handler.h"
#include "../../util/global.h"


char * create_mmap_file(int fd, size_t * size) {
    char * mmap_file = NULL;
    struct stat stat_buf = {0};

    if (fstat(fd, &stat_buf)) {
        throw_and_exit(FILE_STATS_EXCEPTION, "");
    }

    * size = (size_t) stat_buf.st_size;

    if ((mmap_file = mmap(0, (size_t) stat_buf.st_size, PROT_WRITE , MAP_SHARED, fd, 0)) == (caddr_t) - 1) {
        throw_and_exit(MMAP_CREATE_EXCEPTION,"");
    }

    return mmap_file;
}

int write_mmap(char * mmap_addr, size_t * curr_size, size_t * max_size, int fd, char * buffer, size_t buf_size) {
    size_t old_size = * curr_size;

    // check if there is enough space on the current mapping to store the data to be written
    if (* curr_size + buf_size >= * max_size) {
        //duplicate the memory mapping space
        * max_size *= MMAP_FILE_RESIZE_FACTOR;

        //expand the file size to the actual space that is going to be needed when performing sync or unmap operations.
        if (ftruncate(fd, (off_t) max_size) != 0) {
            throw_and_exit(FILE_TRUNC_EXCEPTION, NULL);
        }

        //remap the current memory mapping
        if ((mmap_addr = mremap(mmap_addr, old_size, * max_size, MREMAP_MAYMOVE)) == MAP_FAILED) {
            throw_and_exit(MMAP_REMAP_EXCEPTION, NULL);
        }
    }
    //register the increase of the file size
    * curr_size += buf_size;

    //write the contents on the memory mapped file
    memcpy(mmap_addr + old_size, buffer, * curr_size - old_size);

    return EXIT_SUCCESS;
}

int destroy_mmap_file(char * mmap, int fd, size_t file_size) {

    if (munmap(mmap, file_size) < 0) {
        throw_and_exit(MMAP_DESTROY_EXCEPTION, NULL);
    }

    if (ftruncate(fd, (off_t) file_size) != 0) {
        throw_and_exit(FILE_TRUNC_EXCEPTION, NULL);
    }

    return EXIT_SUCCESS;
}