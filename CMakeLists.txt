cmake_minimum_required(VERSION 3.9)
#project(PosixRecieveTread)
#project(PosixQueueCreate)
#project(PosixQueueSend)
#project(PosixQueueDelete)
project(WriteProcShmem)
project(ReadProcShmem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -std=c++11 -Wno-long-long")


set(SHMEM_DIR /home/user/tests/LockedShmemTest/)

include_directories(/home/user/tests/LockedShmemTest/)

#add_executable(PosixRecieveTread main.cpp POSIX_Queue.cpp POSIX_Queue.h)
#add_executable(PosixQueueCreate creat_queue.cpp POSIX_Queue.h)
#add_executable(PosixQueueSend send_msgs.cpp POSIX_Queue.h)
#add_executable(PosixQueueDelete delete_queue.cpp POSIX_Queue.h)
add_executable(WriteThredTest write_shmem_proc.cpp write_read_test.h write_read_test.cpp)
add_executable(ReadProcShmem read_shmem_proc.cpp write_read_test.h write_read_test.cpp)

#target_link_libraries(PosixRecieveTread rt)
#target_link_libraries(PosixQueueCreate rt)
#target_link_libraries(PosixQueueSend rt)
#target_link_libraries(PosixQueueDelete rt)
target_link_libraries(WriteThredTest rt)
target_link_libraries(ReadProcShmem rt)