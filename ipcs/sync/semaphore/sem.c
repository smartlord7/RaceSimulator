#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "fcntl.h"
#include "semaphore.h"
#include "../../../util/exception_handler/exception_handler.h"
#include "../../../util/debug/debug.h"
#include "sem.h"

sem_t * create_sem(const char * sem_name, int initial_value) {
    assert(sem_name != NULL && initial_value >= 0);

    sem_t * sem;
    sem_unlink(sem_name);

    throw_if_exit((sem = sem_open(sem_name, O_CREAT | O_EXCL, USER_PERMS_ALL, initial_value)) == SEM_FAILED, SEM_CREATE_EXCEPTION, sem_name);

    return sem;
}

void destroy_sem(const char * sem_name, sem_t * sem) {
    assert(sem_name != NULL && sem != NULL);

    throw_if_exit(sem_close(sem) == -1, SEM_CLOSE_EXCEPTION, sem_name);

    DEBUG_MSG(SEM_CLOSE, DEBUG_LEVEL_SETUP, sem_name)

    throw_if_exit(sem_unlink(sem_name) == -1, SEM_UNLINK_EXCEPTION, sem_name);

    DEBUG_MSG(SEM_UNLINK, DEBUG_LEVEL_SETUP, sem_name);
}

sem_t ** create_sem_array(int num, const char * sem_name_prefix, int initial_value) {
    assert(num != 0 && sem_name_prefix != NULL && initial_value > 0);

    sem_t ** sem_array;

    throw_if_exit((sem_array = (sem_t **) malloc(num * sizeof(sem_t *))) == NULL, MEMORY_ALLOCATE_EXCEPTION, SEM_ARRAY);

    int i = 0;

    while (i < num) {
        char sem_name[MAX_SEM_LABEL_SIZE];
        snprintf(sem_name, sizeof(sem_name), "%s%d", sem_name_prefix, i);

        sem_array[i] = create_sem(sem_name, initial_value);

        DEBUG_MSG(SEM_CREATE, DEBUG_LEVEL_SETUP, sem_name)

        i++;
    }

    return sem_array;
}

void destroy_sem_array(sem_t ** sem_array, int num, const char * sem_name_prefix) {
    assert(sem_array != NULL && num != 0 && sem_name_prefix != NULL);

    int i = 0;

    while (i < num) {
        char sem_name[MAX_SEM_LABEL_SIZE];
        snprintf(sem_name, sizeof(sem_name), "%s%d", sem_name_prefix, i);

        destroy_sem(sem_name, sem_array[i]);

        i++;
    }

    free(sem_array);
}

void wait_sem(sem_t * sem, const char * sem_name) {
    assert(sem != NULL && sem_name != NULL);

    throw_if_exit(sem_wait(sem) == -1, SEM_WAIT_EXCEPTION, sem_name);
}

void post_sem(sem_t * sem, const char * sem_name) {
    assert(sem != NULL && sem_name != NULL);

    throw_if_exit(sem_post(sem) == -1, SEM_POST_EXCEPTION, sem_name);
}
