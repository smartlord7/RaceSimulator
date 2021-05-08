#ifndef IPCS_MUTEX_H
#define IPCS_MUTEX_H

#define true 1
#define false 0

typedef pthread_mutex_t mutex_t;

extern void init_mutex(mutex_t * mutex, int proc_shared);
extern void destroy_mutex(mutex_t * mutex);
extern void lock_mutex(mutex_t * mutex);
extern void unlock_mutex(mutex_t * mutex);

#endif //IPCS_MUTEX_H
