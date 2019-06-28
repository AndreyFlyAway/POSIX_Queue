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

void threadFunc(union sigval sv);
int notifySetup(mqd_t *mqdp);
#endif //POSIX_QUEUE_POSIX_QUEUE_H
