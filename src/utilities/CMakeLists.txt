cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 23)

project(TimeTracker)

set(SRCS
        Utilities.cpp
        Utilities.h
)

add_library(utilities STATIC ${SRCS})
add_library(TimeTracker::utilities ALIAS utilities)

target_include_directories(utilities PUBLIC ${CMAKE_CURRENT_BINARY_DIR})