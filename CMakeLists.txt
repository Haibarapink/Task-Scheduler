cmake_minimum_required(VERSION 3.22)
project(scheduler)

set(CMAKE_CXX_STANDARD 17)

add_executable(scheduler main.cc)
add_executable(test_job tests/test_job.cc)
