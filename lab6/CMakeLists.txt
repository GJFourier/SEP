cmake_minimum_required(VERSION 3.21)
project(lab6)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab6
        boggle.cpp
        lexicon.cpp
        lexicon.h)
