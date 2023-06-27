cmake_minimum_required(VERSION 3.21)
project(lab3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab3
        board.cpp
        board.h
        canvas.h
        hanoi.cpp
        queue.h
        stack.h)
