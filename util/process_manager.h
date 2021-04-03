#ifndef GLOBAL_HEADER_H_PROCESS_MANAGER_H
#define GLOBAL_HEADER_H_PROCESS_MANAGER_H

#include <pthread.h>

void create_process(const char * proc_name, void (* worker)(void *), void * params);
void wait_all();
void create_thread(const char * thread_name, pthread_t * thread_p, void * (* worker)(void *), void * params);
void kill_all_threads(int num_threads, pthread_t * threads);

#endif //GLOBAL_HEADER_H_PROCESS_MANAGER_H