//
// Created by user on 6/25/19.
//

#include "POSIX_Queue.h"

int main()
{
    mqd_t mqd;
//    queue_test();
//    del_queue("/mq-l2");
    const char q_name[] = "/mq-l";
    struct mq_attr attr;
//    q_attr.mq_msgsize = sizeof(ShMem);
    int flags = O_RDWR | O_CREAT | O_NONBLOCK;
    mode_t perms = (S_IRUSR | S_IWUSR);

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;
    mqd = mq_open(q_name, O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attr);

//     TODO: check if queue exists already
    if (mqd == (mqd_t) -1)
    {
        printf("ERROR: cant create queue with name %s\n", q_name);
        return -1;
    }
//    mqd_t mqd1 = mq_open(q_name, O_RDWR);

    char msg[32] = "kek";
    if (mq_send(mqd, msg, strlen(msg), 1) == -1)
        printf("ERROR: cant send message, code %d\n", mqd);

    char buf[32] = "kek";
    if (mq_receive(mqd, buf, attr.mq_msgsize, 0) == -1)
        printf("ERROR: cant receive message, code %d\n", mqd);
    else
        printf("Receive res: %s", buf);

    mq_close(mqd);
    return 0;
}

