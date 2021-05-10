#ifndef UTIL_FILE_H
#define UTIL_FILE_H

#define true 1
#define false 0

extern int open_file(const char * file_name, int mode);
extern void close_fd(int fd);
extern void read_stream(int fd, void * strct, size_t size);
extern void write_stream(int fd, void * buffer, size_t size);

#endif //UTIL_FILE_H
