cmake_minimum_required(VERSION 3.25)
project(TimeTracker)

set(CMAKE_CXX_STANDARD 23)

add_executable(TimeTracker src/main.cpp src/ActivityMonitor.cpp src/ActivityMonitor.h)
target_link_libraries(TimeTracker Version.lib)

