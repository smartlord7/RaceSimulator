/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 18/05/2021
*/

#ifndef IPCS_SYNC_COND_VAR_H
#define IPCS_SYNC_COND_VAR_H

#include "pthread.h"
#include "../mutex/mutex.h"

typedef pthread_cond_t cond_t;

extern void wait_cond(cond_t * cond, mutex_t * mutex);
extern void init_cond(cond_t * cond, int proc_shared);
extern void destroy_cond(cond_t * cond);
extern void notify_cond_all(cond_t * cond);
extern void notify_cond(cond_t * cond);

#endif //IPCS_SYNC_COND_VAR_H
