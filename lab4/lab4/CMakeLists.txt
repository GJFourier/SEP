cmake_minimum_required(VERSION 3.21)
project(lab4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/1.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/2.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/3.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(lab4
        main.cpp
        Tree.cpp
        )
