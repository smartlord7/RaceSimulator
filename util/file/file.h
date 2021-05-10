#ifndef UTIL_FILE_H
#define UTIL_FILE_H

#define true 1
#define false 0

extern int open_file(const char * file_name, int mode);
extern void close_fd(int fd);

#endif //UTIL_FILE_H
