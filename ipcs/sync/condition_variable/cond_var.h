#ifndef IPCS_COND_VAR_H
#define IPCS_COND_VAR_H

#define true 1
#define false 0

extern void set_sh_cond_var(pthread_cond_t * cond);
extern void destroy_cond_var(pthread_cond_t * cond);

#endif //IPCS_COND_VAR_H
