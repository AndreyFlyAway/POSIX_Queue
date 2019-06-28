//
// Created by user on 6/27/19.
//
#include "POSIX_Queue.h"

int main(void)
{
    mqd_t mqd;
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;
    mqd = mq_open(Q_NAME, O_RDWR | O_CREAT | O_NONBLOCK, 0644, &attr);

    if (mqd == (mqd_t) -1)
    {
        printf("ERROR: cant create queue with name %s\n", Q_NAME);
        return -1;
    }
    return 0;
}