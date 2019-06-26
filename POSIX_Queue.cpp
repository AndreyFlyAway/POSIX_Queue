//
// Created by user on 6/24/19.
//

#include "POSIX_Queue.h"

int creat_queue(mqd_t *mqd, const char *mq_name, int flags, u_int mq_msg_size)
{
    struct mq_attr q_attr;
    q_attr.mq_msgsize = mq_msg_size;
    mode_t perms = (S_IRUSR | S_IWUSR);
    *mqd = mq_open(mq_name, flags, perms, q_attr);
    // TODO: check if queue exists already
    if ((*mqd) == (mqd_t) -1)
    {
        printf("ERROR: cant create queue with name %s\n", mq_name);
        return -1;
    } else
        return (int)(*mqd);
}

int del_queue(const char * q_name)
{
    mqd_t mqd = mq_open(q_name, O_RDWR);
    int res = mq_close(mqd);
    printf("Delete res %d", res);
    return res;
}

int queue_test(void)
{
    mqd_t mqd;
//    char q_name[] = "/mq-l";
//
//    if (creat_queue(&mqd, q_name) == -1)
//    {
//        return -1;
//    }
    const char q_name[] = "/mq-l2";

    struct mq_attr q_attr;
    q_attr.mq_msgsize = 14;
    q_attr.mq_maxmsg = 32;
    q_attr.mq_curmsgs = 0;
    q_attr.mq_flags = 0;

    int flags = O_CREAT | O_RDWR | O_NONBLOCK;
    mode_t perms = (S_IRUSR | S_IWUSR);
    mqd = mq_open(q_name, O_RDWR | O_CREAT | O_NONBLOCK , 0644, &q_attr);
//     TODO: check if queue exists already
    if (mqd == (mqd_t) -1)
    {
        printf("ERROR: cant create queue with name %s\n", q_name);
        return -1;
    }


    struct ShMem shmem_q_l[MSGS_NUM];
    for(auto i = 0 ; i < MSGS_NUM ; i++)
    {
        sprintf(shmem_q_l[i].ch_data, "test_%d", i);
        shmem_q_l[i].ui_data = (uint)i;
        shmem_q_l[i].fl_data = (float)i;
    }
    // put some message into the queue
    mqd_t mqd_write = mq_open(q_name, O_WRONLY);
    for(auto i = 0 ; i < MSGS_NUM ; i++)
    {
        if(mq_send(mqd_write, (char*)(shmem_q_l+i), sizeof(ShMem)+1, 0) == -1)
//        if(mq_send(mqd, shmem_q_l[i].ch_data, sizeof(ShMem), 0) == -1)
        {
            printf("ERROR: cant send message into, code %d\n", mqd);
        }
    }

    // read some message in the queue
    struct ShMem shmem_buf;
//    void *buff;
//    buff = malloc(sizeof(ShMem));
//    if (buff == NULL)
//    {
//        printf("ERROR: cant call malloc to create buffer for queue %d", q_name);
//        return -1;
//
//    }
    mqd_t mqd_read = mq_open(q_name, O_RDONLY);
    for(auto i = 0 ; i < MSGS_NUM ; i++)
    {
//        mq_receive(mqd, (char*)(buff), sizeof(ShMem), 0);
        if (mq_receive(mqd_read, (char*)(&shmem_buf), sizeof(ShMem), 0) == -1){
            printf("ERROR: cant receive message from queue %s\n", q_name);
//            return -1;
        } else
        {
            printf("Message from queue : %s, %d, %f\n", shmem_buf.ch_data, shmem_buf.ui_data, shmem_buf.fl_data);
        }

    }
    mq_close(mqd);

    return 0;
}