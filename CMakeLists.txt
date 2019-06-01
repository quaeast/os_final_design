cmake_minimum_required(VERSION 3.14)
project(os_final_design)

set(CMAKE_CXX_STANDARD 14)

add_executable(deamon deamon.cpp config.h)
add_executable(clinet clinet.cpp config.h)

