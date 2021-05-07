#ifndef IPCS_MSG_QUEUE_H
#define IPCS_MSG_QUEUE_H

#define true 1
#define false 0

extern int create_msg_queue();
extern int rcv_msg(int msgq_id, void * msg, size_t msg_size, long type);
extern void snd_msg(int msgq_id, void * msg, size_t msg_size);
extern void destroy_msg_queue(int msgq_id);

#endif //IPCS_MSG_QUEUE_H
