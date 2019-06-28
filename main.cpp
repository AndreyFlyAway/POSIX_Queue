//
// Created by user on 6/25/19.
//

#include "POSIX_Queue.h"
#include <unistd.h>

int main()
{
    mqd_t mqd;

    mqd = mq_open(Q_NAME, O_RDONLY | O_NONBLOCK);
    if (mqd == (mqd_t) -1)
    {
        printf("ERROR: cant open %s", Q_NAME);
        return -1;
    }
    notifySetup(&mqd);
    pause();
//    usleep(10000000);

    printf("Done");
}

