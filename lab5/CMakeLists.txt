cmake_minimum_required(VERSION 3.21)
project(lab5)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab5
        cuckoo.cpp
        cuckoo.h
        main.cpp)
