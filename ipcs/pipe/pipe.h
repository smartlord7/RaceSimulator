#ifndef IPCS_NAMED_PIPE_H
#define IPCS_NAMED_PIPE_H

#define true 1
#define false 0

extern void create_unn_pipe(int fds[2]);
extern void close_fds(const char * file_type, const int * fds, int num_fds);
extern void create_named_pipe(const char * pipe_name, int * fd, int mode);

#endif //IPCS_NAMED_PIPE_H
