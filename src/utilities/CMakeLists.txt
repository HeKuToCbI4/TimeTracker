cmake_minimum_required(VERSION 3.23)
project(utilities)

set(CMAKE_CXX_STANDARD 23)

set(SRCS
        Utilities.cpp
        Utilities.h
)

add_library(utilities STATIC ${SRCS})
