/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 18/05/2021
*/

// region dependencies

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "errno.h"
#include "sys/msg.h"
#include "msg_queue.h"
#include "../../util/exception_handler/exception_handler.h"

// endregion dependencies

// region public functions

int create_msg_queue() {
    int msgq_id;

    msgq_id = msgget(IPC_PRIVATE, IPC_CREAT | 0777);

    throw_if_exit(msgq_id == -1, MSG_QUEUE_CREATE_EXCEPTION, NULL);

    return msgq_id;
}

int rcv_msg(int msgq_id, void * msg, size_t msg_size, long type) {
    assert(msg != NULL);
    int nread;

    nread = (int) msgrcv(msgq_id, msg, msg_size - sizeof(long), type, IPC_NOWAIT);

    throw_if_exit(errno != ENOMSG && errno != 0, MSG_QUEUE_RECEIVE_EXCEPTION, msgq_id);

    return nread;
}

void snd_msg(int msgq_id, void * msg, size_t msg_size) {
    assert(msg != NULL);

    throw_if_exit(msgsnd(msgq_id, msg, msg_size - sizeof(long), 0) == -1, MSG_QUEUE_SEND_EXCEPTION, msgq_id);
}

void destroy_msg_queue(int msgq_id) {

    throw_if_exit(msgctl(msgq_id, IPC_RMID, NULL) == -1, MSG_QUEUE_DESTROY_EXCEPTION, msgq_id);
}

// endregion public functions