//
// Created by joao on 14/05/21.
//

#ifndef MESSAGE_QEUE_C_MEMORY_MAPPED_FILE_H
#define MESSAGE_QEUE_C_MEMORY_MAPPED_FILE_H

char * create_mmap_file(int fd, int offset, size_t * size);
int write_mmap(char * mmap_addr, char * buffer, int fd, size_t * file_size);
int destroy_mmap(char * mmap, int fd, size_t file_size);

#endif //MESSAGE_QEUE_C_MEMORY_MAPPED_FILE_H
