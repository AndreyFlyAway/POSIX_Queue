cmake_minimum_required(VERSION 3.9)
project(PosixQueueLern)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -std=c++11 -Wno-long-long")


set(SHMEM_DIR /home/user/tests/LockedShmemTest/)

include_directories(/home/user/tests/LockedShmemTest/)

add_executable(PosixQueueLern main.cpp POSIX_Queue.cpp POSIX_Queue.h)

target_link_libraries(PosixQueueLern rt)