//
// Created by user on 7/2/19.
//

#include "write_read_test.h"

int main()
{
    uint64_t ts_start {};
    uint64_t ts_process_mcs;
    ts_process_mcs = get_timestamp_ns()/1000;
    ShMem *shmem_p;

    uint64_t work_time {15};
    createShMem(0, &(shmem_p));
    while((get_timestamp_ns()/1000 - ts_process_mcs) < (15 * 1000000)){
        write_proc_single(shmem_p);
//        while_ts = ts_process_mcs - (get_timestamp_ns()/1000);
    }
    freeShMem(shmem_p);

//    read_proc_single(shmem_p, last_tpdo_ts, ts_process_mcs, next_process_ts);
}