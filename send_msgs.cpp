//
// Created by user on 6/27/19.
//

#include "POSIX_Queue.h"

int main()
{
    mqd_t mqd;
    struct ShMem shmemObj;
    mqd = mq_open(Q_NAME, O_WRONLY | O_NONBLOCK);
    if (mqd == (mqd_t) -1)
    {
        printf("ERROR: cant open queue for sending %s\n", Q_NAME);
        return -1;
    }
//    char message[] = "lol 123 567";
    for(int i = 0 ; i < 10 ; i ++) {
        sprintf(shmemObj.ch_data, "Char_%d", i);
        shmemObj.fl_data = (uint)i;
        shmemObj.fl_data = (float)i;
        if (mq_send(mqd, (char *) (&shmemObj), sizeof(shmemObj), 0) == -1)
            printf("ERROR: cant send message, code %d\n", mqd);
    }
    return 0;
}