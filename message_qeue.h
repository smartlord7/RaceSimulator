//
// Created by jfgar on 30/04/2021.
//

#ifndef IPC_MANAGER_H_MESSAGE_QEUE_H

#define MESSAGE_SIZE sizeof(message_t) - sizeof(long)

typedef struct message_t message_t;
struct message_t {
    long priority;
    int type_of_malfunction;
};

int create_msg_queue();
int destroy_message_queue(int msgq_id);
int receive_message(int msgq_id, message_t * message, int priority);
int send_message(int msgq_id, message_t * message);

#define IPC_MANAGER_H_MESSAGE_QEUE_H

#endif //IPC_MANAGER_H_MESSAGE_QEUE_H
