cmake_minimum_required(VERSION 3.23)
project(TimeTracker)

set(CMAKE_CXX_STANDARD 23)

find_package(Protobuf REQUIRED)

add_subdirectory(proto)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${Protobuf_INCLUDE_DIRS})

add_executable(TimeTracker src/main.cpp src/ActivityMonitor.cpp src/ActivityMonitor.h src/ActivitySnapshot.h src/ActivitySnapshot.cpp)

target_link_libraries(TimeTracker proto ${Protobuf_LIBRARIES} Version.lib)