#ifndef GLOBAL_HEADER_H_PROCESS_MANAGER_H
#define GLOBAL_HEADER_H_PROCESS_MANAGER_H

void create_process(const char * proc_name, void (* worker)(void *), void * params);
void wait_all();

#endif //GLOBAL_HEADER_H_PROCESS_MANAGER_H
