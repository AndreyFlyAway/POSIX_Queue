//
// Created by user on 6/24/19.
//

#include "POSIX_Queue.h"

void threadFunc(union sigval sv) {
    ssize_t bytesRead;
    mqd_t *mqdp;
    void *buff;
//    struct ShMem shm;
    struct ShMem *shmp;
    struct mq_attr attr;

    mqdp = (mqd_t *) sv.sival_ptr;
    if (mq_getattr(*mqdp, &attr) == -1) {
        printf("ERROR: cant get attributes from queue with descriptor %d", (int)(*mqdp));
        pthread_exit(NULL);
    }
    buff = malloc(attr.mq_msgsize);
    if (buff == NULL) {
        printf("ERROR: cant call malloc for buff with size %d", (int)attr.mq_maxmsg);
        pthread_exit(NULL);
    }

    notifySetup(mqdp);

    while ((bytesRead = mq_receive(*mqdp, (char*)buff, attr.mq_msgsize, NULL)) >= 0)
    {
        shmp = (ShMem*)(buff);
//        memcpy((void *)(&shm), buff, sizeof(shm));
        printf("Read %ld bytes: %s %u %f\n", (long)bytesRead, shmp->ch_data, shmp->ui_data, shmp->fl_data);
//        printf("Read %ld bytes: %s %u %f", (long)bytesRead, shm.ch_data, shm.ui_data, shm.fl_data);
    }
    free(buff);
//    notifySetup(mqdp);
//    pthread_exit(NULL);
}


int notifySetup(mqd_t *mqdp)
{
    struct sigevent sigEv;
    sigEv.sigev_notify = SIGEV_THREAD;
    sigEv.sigev_notify_function = threadFunc;
    sigEv.sigev_notify_attributes = NULL;
    sigEv.sigev_value.sival_ptr = mqdp;
    if (mq_notify(*mqdp, &sigEv))
        return - 1;
    return 0;
}

/* write function for work with shared memory */
int write_thread(msg_test * msgs_arr) {
    msg_test *msg_p;
    uint64_t ts;
    for (int i = 0; i < MSGS_NUM; i++){
        // catch a data mutex
        if (sem_wait(&msg_p->mutex) == -1)
        {
            printf("! !sem_wait error\n");

            return SHMEM_SEMWAIT_ERROR;
        }
        msg_p = msgs_arr + i;
        ts = get_timestamp_ns();
        msg_p->ts = ts;
        msg_p->message = (uint64_t)(i + 100);
        // free the data mutex
        if (sem_post(&msg_p->mutex) == -1)
        {
            printf("! !sem_post error\n");
            return SHMEM_SEMPOST_ERROR;
        }
    }

}