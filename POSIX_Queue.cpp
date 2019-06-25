//
// Created by user on 6/24/19.
//

#include "POSIX_Queue.h"

int creat_queue(mqd_t *mqd, const char *mq_name, int flags, u_int mq_msg_size)
{
    struct mq_attr q_attr;
    q_attr.mq_msgsize = mq_msg_size;
    *mqd = mq_open(mq_name, flags, q_attr);
    if ((*mqd) == (mqd_t) -1)
    {
        printf("ERROR: cant create queue with name %s", mq_name);
        return -1;
    } else
        return (int)(*mqd);
}

int queue_test(void)
{
    char q_name[] = "/mq-l";
    int flags = O_RDWR | O_EXCL;
    struct mq_attr q_attr;
    q_attr.mq_msgsize = sizeof(ShMem);
    mqd_t mqd;

    mqd = mq_open(q_name, flags, q_attr);
    if (mqd == (mqd_t) -1)
    {
        printf("ERROR: cant create queues with name %s", q_name);
        return -1;
    }

    return 0;
}