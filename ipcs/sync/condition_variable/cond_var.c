#include "assert.h"
#include "pthread.h"
#include "../../../util/debug/exception_handler.h"
#include "cond_var.h"

void set_sh_cond_var(pthread_cond_t * cond) {
    assert(cond != NULL);

    pthread_condattr_t attr;

    if (pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0) {
        throw_and_exit(COND_VAR_SHARE_EXCEPTION, "");
    }

    if (pthread_cond_init(cond, &attr) != 0) {
        throw_and_exit(COND_VAR_INITIALIZE_EXCEPTION, "");
    }
}

void destroy_cond_var(pthread_cond_t * cond) {
    assert(cond != NULL);

    if (pthread_cond_destroy(cond) != 0) {
        throw_and_exit(COND_VAR_DESTROY_EXCEPTION, "");
    }
}
