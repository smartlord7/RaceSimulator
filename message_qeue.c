//
// Created by jfgar on 30/04/2021.
//

#include "message_qeue.h"
#include "exception_handler.h"
#include <stdlib.h>
#include <sys/msg.h>

#define true 1
#define false 0

int create_message_queue() {
    int msgq_id;

    msgq_id = msgget(IPC_PRIVATE, IPC_CREAT | 0777);
    if(msgq_id < 0){
        throw_exception_and_exit(MSGQ_CREATION_EXCEPTION, NULL);
    }

    return msgq_id;
}

int receive_message(int msgq_id, message_t * message, int priority) {

    if(msgrcv(msgq_id, message, MESSAGE_SIZE, priority, 0) < 0){
        throw_exception_and_stay(MSG_RECEPTION_EXCEPTION, NULL);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int send_message(int msgq_id, message_t * message) {

    if(msgsnd(msgq_id, message, MESSAGE_SIZE, 0) < 0){
        throw_exception_and_stay(MSG_EMISSION_EXCEPTION, NULL);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int destroy_message_queue(int msgq_id) {

    if(msgctl(msgq_id, IPC_RMID, NULL) < 0) {
        throw_exception_and_stay(MSGQ_DESTRUITION_EXCEPTION, NULL);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
