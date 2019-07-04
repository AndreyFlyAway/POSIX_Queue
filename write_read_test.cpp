//
// Created by user on 6/27/19.
//
#include "write_read_test.h"

/* get timestamp */
int createShMem(int id, ShMem** ptr, int locked, char* shmem_name_fmt)
{
    char shmem_name[32];
    ::sprintf(shmem_name, shmem_name_fmt, id);
//    std::cout << "Creating shmem with name " << shmem_name << std::endl;
    // creating shared memory
    ::shm_unlink(shmem_name);
    int fd = ::shm_open(shmem_name, O_RDWR | O_CREAT | O_EXCL | O_NONBLOCK, FILE_MODE);

    if (fd < 0) {
        printf("Error: shm_open eeprog /dev/i2c-4 0x57 -16 -f -r 0x0 for %s. Return code: %d\n", shmem_name, fd);
        return RC_SHMEM_OPEN_ERR;
    }

    if (ptr) {
        *ptr = (ShMem *) ::mmap(NULL, sizeof(ShMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    } else {
        printf("Error: mmap NULL pointer for %s.\n", shmem_name);
        return RC_BAD_POINTER_ERR;
    }

    if (*ptr == MAP_FAILED) {
        printf("ERROR: mmap faild %s", shmem_name);
        return RC_MAP_FAILD_ERR;
    }

    if (::ftruncate(fd, sizeof(ShMem)) == -1) {
        printf("ERROR: mmap failed %s", shmem_name);
        return RC_FTRUNCATE_ERR;
    }

    if (::close(fd) == -1) {
        printf("ERROR: close by file descriptor of shared memory failed %s, fd: %d", shmem_name, fd);
        return RC_CLOSE_FD_ERR;
    }
    // semaphore initialization
    if (::sem_init(&(*ptr)->data_mutex, 1, 1) == -1)
    {
        printf("ERROR: failed %s", shmem_name);
        return RC_SEM_INIT_ERR;
    }

    // block shared memory
    if (locked == 1)
        if (mlock(ptr, sizeof(ShMem)) == -1)
        {
            printf("! ! mlock error for %s: %d (%s)\n", shmem_name, errno, strerror(errno));
            return RC_SHMEM_MLCOK_ERR;
        }

    return RC_SHMEM_OK;
}

int getShMem(int id, ShMem** ptr, char* shmem_name_fmt)
{
    char shmem_name[32];
    ::sprintf(shmem_name, shmem_name_fmt, id);
//    std::cout << "Writting shmem with name " << shmem_name << std::endl;

    int fd = ::shm_open(shmem_name, O_RDWR, FILE_MODE);
    if (fd < 0)
    {
        printf("Error: shm_open failed for %s. Return code: %d\n", shmem_name, fd);
        return RC_SHMEM_OPEN_ERR;
    }

    if (ptr) {
        *ptr = (ShMem *) ::mmap(NULL, sizeof(ShMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    } else {
        printf("Error: mmap NULL pointer for %s.\n", shmem_name);
        return RC_BAD_POINTER_ERR;
    }

    if (*ptr == MAP_FAILED) {
        printf("ERROR: mmap faild %s", shmem_name);
        return RC_MAP_FAILD_ERR;
    }

    if (::close(fd) == -1) {
        printf("ERROR: close by file descriptor of shared memory failed %s, fd: %d", shmem_name, fd);
        return RC_CLOSE_FD_ERR;
    }

    return RC_SHMEM_OK;
}

int freeShMem(ShMem* ptr)
{
    if(ptr)
    {
        if(::munmap(ptr, sizeof(ShMem))<0)
        {
            printf("ERROR: freeShMem faild: %d (%s)\n", errno, strerror(errno));
            return RC_MAP_FAILD_ERR;
        }
    }

    return RC_SHMEM_OK;
}

int releaseShMem(int id, char* shmem_name_fmt)
{
    char shmem_name[32];
    ::sprintf(shmem_name, shmem_name_fmt, id);

    return ::shm_unlink(shmem_name);
}

uint64_t get_timestamp_ns()
{
    uint64_t ts;
    timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    ts = (uint64_t)(spec.tv_sec) * 1000000000 + (uint64_t)(spec.tv_nsec);

    return ts;
}

/* write function for work with array of pointer to shared memory */
int write_proc_single(ShMem * msg_p) {
    uint64_t ts;
    // catch a data mutex
//    printf("write_proc_single\n");
    printf("w p 1\n");
    if (sem_wait(&(msg_p->data_mutex)) == -1)
    {
        printf("! !sem_wait error\n");

        return RC_SHMEM_SEMWAIT_ERR;
    }
    printf("w p 2\n");
//    timespec spec;
//    clock_gettime(CLOCK_MONOTONIC, &spec);
//    msg_p->ts = (uint64_t)(spec.tv_sec) * 1000 + (uint64_t)(spec.tv_nsec) / 1000000;
    ts = get_timestamp_ns();
    msg_p->ts = ts/1000;
    msg_p->message = (uint64_t)(rand());
    usleep(10000);
    // free the data mutex
    if (sem_post(&(msg_p->data_mutex)) == -1)
    {
        printf("! !sem_post error\n");
        return RC_SHMEM_SEMPOST_ERR;
    }
    printf("w p 3\n");
    return 0;
}

/* read function for work with shared memory */
int read_proc_single(ShMem *msg_p, uint64_t &last_tpdo_ts, uint64_t &ts_process_mcs, uint64_t &next_process_ts) {
    uint64_t d {};
//    printf("read_proc_single_calss\n");
    if (sem_wait(&(msg_p->data_mutex)) == -1)
    {
        printf("! !sem_wait error\n");
        return RC_SHMEM_SEMWAIT_ERR;
    }

    uint64_t msg_ddata = msg_p->message;

    if (sem_post(&(msg_p->data_mutex)) == -1)
    {
        printf("! !sem_post error\n");
        return RC_SHMEM_SEMPOST_ERR;
    }

//    printf("r p 1\n");
//    printf("d 2 %d\n", (ts_process_mcs ) >= msg_p->ts);
    if((last_tpdo_ts < msg_p->ts) && ((ts_process_mcs/1000 ) >= (msg_p->ts/1000))){
//        printf("r p 2\n");
        d = (ts_process_mcs) - msg_p->ts;
        last_tpdo_ts = ts_process_mcs/1000;
        printf("Processing last with val %llu :  %llu delta: %llu\n", msg_ddata, last_tpdo_ts, d);
    }
    if(ts_process_mcs > next_process_ts)
    {
//        printf("r p 3\n");
        next_process_ts = ts_process_mcs + PROCESS_TIME_PERIOD_MCS*1000;
    }
//    printf("r p 4\n");
    return 0;
}

/* write function for work with array of pointer to shared memory */
int write_proc_arr(ShMem * msgs_arr[]) {
    ShMem *msg_p;
    uint64_t ts;
    for (int i = 0; i < MSGS_NUM; i++){
        // catch a data mutex
        msg_p = msgs_arr[i];
        if (sem_wait(&msg_p->data_mutex) == -1)
        {
            printf("! !sem_wait error\n");

            return RC_SHMEM_SEMWAIT_ERR;
        }
        ts = get_timestamp_ns();
        msg_p->ts = ts/1000;
        msg_p->message = (uint64_t)(i + 100);
        // free the data mutex
        if (sem_post(&msg_p->data_mutex) == -1)
        {
            printf("! !sem_post error\n");
            return RC_SHMEM_SEMPOST_ERR;
        }
    }
    return 0;
}
//
//int main()
//{
//    // create blocks of shared memory
//    ShMem *shmem_arr[MSGS_NUM];
//    for (int i = 0; i < MSGS_NUM; i++) {
//        createShMem(i, &(shmem_arr[i]));
//    }
//    write_proc_arr(&(shmem_arr[0]));
//    // free blocks of shared memory
//    for (int i = 0; i < MSGS_NUM; i++) {
//        printf("r %d %llu\n", i, shmem_arr[i]->message);
//    }
//    for (int i = 0; i < MSGS_NUM; i++) {
//        freeShMem(shmem_arr[i]);
//    }
//    return 0;
//}