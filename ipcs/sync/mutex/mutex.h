#ifndef IPCS_MUTEX_H
#define IPCS_MUTEX_H

#define true 1
#define false 0

extern void set_sh_mutex(pthread_mutex_t * mutex);
extern void destroy_mutex(pthread_mutex_t * mutex);

#endif //IPCS_MUTEX_H
