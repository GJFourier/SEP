cmake_minimum_required(VERSION 3.20)
project(lab1)

set(CMAKE_CXX_STANDARD 11)

add_library(lab1lib Class.h Student.h Class.cc Student.cc)
add_executable(lab1 main.cc)
target_link_libraries(lab1 lab1lib)
