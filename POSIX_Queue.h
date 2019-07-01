//
// Created by user on 6/24/19.
//

#ifndef POSIX_QUEUE_POSIX_QUEUE_H
#define POSIX_QUEUE_POSIX_QUEUE_H

#include <mqueue.h>
#include <pthread.h>
#include <sys/stat.h> /* constants for argument mode (for creating POSIX queue) */
#include <sys/fcntl.h> /* constants of type Q_*/
#include "LockedShMemTest.h"

// constants
#define	MQ_MAX_MSG_NUM	           32
#define	MSGS_NUM    	           16
#define	Q_NAME    	               "/mq-l"
#define	PROCESS_TIME_PERIOD_MS    	   100000


// retun codes
#define	SHMEM_SEMWAIT_ERROR    	   -1
#define	SHMEM_SEMPOST_ERROR    	   -2


struct msg_test {
    uint64_t            ts;
    uint8_t             msg_data[MSGS_NUM];
    uint64_t            message;        // данные для модуля. Очередь не имеет смысла
    sem_t               mutex;          // семафор Posix, защищает доступ к message
};

void threadFunc(union sigval sv);
int notifySetup(mqd_t *mqdp);
void processingTest();
int write_thread(msg_test * msgs_arr) ;

#endif //POSIX_QUEUE_POSIX_QUEUE_H
