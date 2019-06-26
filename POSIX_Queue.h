//
// Created by user on 6/24/19.
//

#ifndef POSIX_QUEUE_POSIX_QUEUE_H
#define POSIX_QUEUE_POSIX_QUEUE_H

#include <mqueue.h>
#include <sys/stat.h> /* constants for argument mode (for creating POSIX queue) */
#include <sys/fcntl.h> /* constants of type Q_*/
#include "LockedShMemTest.h"

// constants
#define	MQ_MAX_MSG_NUM	           32
#define	MSGS_NUM    	           16


int queue_test(void);
int creat_queue(mqd_t *mqd, const char *mq_name, int flags=(O_RDWR | O_EXCL), u_int mq_msg_size=sizeof(ShMem));
int del_queue(const char * q_name);

#endif //POSIX_QUEUE_POSIX_QUEUE_H
