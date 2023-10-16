cmake_minimum_required(VERSION 3.23)
project(TimeTracker)

set(CMAKE_CXX_STANDARD 23)

find_package(Protobuf REQUIRED)
FIND_PACKAGE(gRPC CONFIG REQUIRED)

add_subdirectory(src/utilities)
add_subdirectory(proto)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${Protobuf_INCLUDE_DIRS})

add_executable(TimeTracker
        src/main.cpp
        src/ActivityMonitor.cpp
        src/ActivityMonitor.h
        src/ActivitySnapshot.h
        src/ActivitySnapshot.cpp
        src/FrameInfoService.cpp
        src/FrameInfoService.h
)

target_link_libraries(TimeTracker proto utilities ${Protobuf_LIBRARIES} Version.lib)