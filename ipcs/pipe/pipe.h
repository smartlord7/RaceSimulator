#ifndef IPCS_NAMED_PIPE_H
#define IPCS_NAMED_PIPE_H

#define true 1
#define false 0

extern void create_unn_pipe(int fds[2]);
extern void create_named_pipe(const char * pipe_name);
extern void destroy_named_pipe(const char * pipe_name);

#endif //IPCS_NAMED_PIPE_H
