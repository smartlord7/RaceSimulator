#ifndef RACESIMULATOR_DEBUG_H
#define RACESIMULATOR_DEBUG_H

#include <semaphore.h>

#define SHM_CREATED "CREATED SHARED MEMORY WITH ID %d!"
#define SHM_ATTACHED "ATTACHED SHARED MEMORY WITH ID %d TO CURRENT PROCESS ADDRESSING ZONE!"
#define SEM_CREATED "CREATED SEMAPHORE NAMED %s!"

#define SHM_DETACHED "DETACHED SHARED MEMORY WITH ID %d!"
#define SHM_REMOVED "REMOVED SHARED MEMORY WITH ID %d!"
#define SEM_CLOSED "CLOSED SEMAPHORE NAMED %s!"
#define SEM_UNLINKED "UNLINKED SEMAPHORE NAMED %s!"
#define IPCS_CREATED "CREATED IPCS!"

#define RACE_CONFIG_CREATED "CREATED RACE CONFIGURATION!"
#define RUNNING_PROCESS "RUNNING PROCESS %s..."
#define RUNNING_THREAD "RUNNING THREAD %s..."
#define EXITING_PROCESS "EXITING PROCESS %s..."
#define EXITING_THREAD "EXITING THREAD %s..."

#define DEBUG_MSG(msg, ...) if (DEBUG) debug_msg(__FILE__, __LINE__, msg, __VA_ARGS__);
#define SYNC_DEBUG_MSG(mutex, msg, ...) if (DEBUG) { sem_wait(mutex); debug_msg(__FILE__, __LINE__, msg, __VA_ARGS__); sem_post(mutex); }

void debug_msg(const char * file, int line, const char * msg, ...);

#endif //RACESIMULATOR_DEBUG_H