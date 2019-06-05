cmake_minimum_required(VERSION 3.14)
project(os_final_design)

set(CMAKE_CXX_STANDARD 14)

add_executable(daemon file_system.cpp daemon.cpp  )
add_executable(clinet file_system.cpp clinet.cpp )
add_executable(test file_system.cpp test.cpp )

