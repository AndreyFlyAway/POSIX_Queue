//
// Created by user on 7/1/19.
//

#ifndef WRITE_PROCESS_CPP_H
#define WRITE_PROCESS_CPP_H

//
// Created by user on 6/24/19.
//

#ifndef POSIX_QUEUE_POSIX_QUEUE_H
#define POSIX_QUEUE_POSIX_QUEUE_H

// headers
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/mman.h>
#include <cstdio>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <inttypes.h>
#include <iostream>
#include <mqueue.h>
#include <pthread.h>
#include <sys/stat.h> /* constants for argument mode (for creating POSIX queue) */
#include <sys/fcntl.h> /* constants of type Q_ */
#include <cstdlib> /* for rand */
#include <ctime> /* for rand */

// constants
#define	MSGS_NUM    	               256
#define	Q_NAME    	                   "/mq-l"
#define	PROCESS_TIME_PERIOD_MCS    	   10000
#define	EXPEREMETN_COUNTER	           10
#define BASE_SHMNAME_FMT               "SHMEM_%d"
#define	FILE_MODE	                   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

// retun codes
#define RC_SHMEM_OK                     0
#define RC_SHMEM_OPEN_ERR              -1
#define RC_BAD_POINTER_ERR             -2
#define RC_MAP_FAILD_ERR               -3
#define RC_FTRUNCATE_ERR               -4
#define RC_CLOSE_FD_ERR                -5
#define RC_SEM_INIT_ERR                -6
#define RC_SHMEM_MLCOK_ERR             -7
#define	RC_SHMEM_SEMWAIT_ERR 	       -8
#define	RC_SHMEM_SEMPOST_ERR   	       -9


struct ShMem {
    uint64_t            ts;
    uint8_t             msg_data[MSGS_NUM];
    uint64_t            message;        // данные для модуля. Очередь не имеет смысла
    sem_t               data_mutex;          // семафор Posix, защищает доступ к message
};

int createShMem(int id, ShMem** ptr, int locked=0, char* shmem_name_fmt=(char *)(BASE_SHMNAME_FMT));
int getShMem(int id, ShMem** ptr, char* shmem_name_fmt=(char *)(BASE_SHMNAME_FMT));
int freeShMem(ShMem* ptr);
int releaseShMem(int id, char* shmem_name_fmt);
int notifySetup(mqd_t *mqdp);
void processingTest();
uint64_t get_timestamp_ns();
int write_proc_single(ShMem *msgs_arr);
int write_proc_arr(ShMem *msgs_arr[]);
int read_proc_single(ShMem *msgs_arr, uint64_t &last_tpdo_ts, uint64_t &ts_process_mcs, uint64_t &next_process_ts);

#endif //POSIX_QUEUE_POSIX_QUEUE_H


#endif //WRITE_PROCESS_CPP_H
