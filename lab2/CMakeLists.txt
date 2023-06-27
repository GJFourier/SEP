cmake_minimum_required(VERSION 3.20)
project(lab2)

set(CMAKE_CXX_STANDARD 11)

add_library(lab2lib Buffer.h Editor.h Buffer.cc Editor.cc)
add_executable(lab2 ed.cc)
target_link_libraries(lab2 lab2lib)
