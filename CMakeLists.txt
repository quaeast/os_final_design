cmake_minimum_required(VERSION 3.14)
project(os_final_design)

set(CMAKE_CXX_STANDARD 14)

add_executable(daemon daemon.cpp config.h file_system.h file_system.cpp inode.h dir.h)
add_executable(clinet clinet.cpp config.h inode.h )
add_executable(test test.cpp config.h file_system.h file_system.cpp inode.h dir.h)

