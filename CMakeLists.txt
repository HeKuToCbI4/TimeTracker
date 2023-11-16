cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)

set(VCPKG_HOST_TRIPLET x64-windows)
set(VCPKG_TARGET_TRIPLET x64-windows)

# Get rid of warning.
# set(Boost_NO_WARN_NEW_VERSIONS ON)
# Boost stuff.
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#add_definitions(-DBOOST_ALL_DYN_LINK=1)

project(TimeTracker)

message("Looking for protobuf")
find_package(Protobuf REQUIRED)
message("Looking for grpc")
find_package(gRPC CONFIG REQUIRED)
find_package(Threads)

#message("Looking for boost-log")
#find_package(Boost COMPONENTS log log_setup REQUIRED)
#
#message("Boost VERSION: ${Boost_VERSION}")
#message("Boost INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#message("Boost Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
#message("Boost LIBRARIES: ${Boost_LIBRARIES}")

add_subdirectory(src/utilities)
add_subdirectory(src/protobuf)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
)
#include_directories(${Boost_INCLUDE_DIRS})

add_executable(TimeTracker
        src/main.cpp
        src/ActivityMonitor.cpp
        src/ActivityMonitor.h
        src/ActivitySnapshot.h
        src/ActivitySnapshot.cpp
        src/FrameInfoService.cpp
        src/FrameInfoService.h
)

target_link_libraries(TimeTracker
        TimeTracker::proto
        TimeTracker::utilities
#        ${Boost_LIBRARIES}
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
        Threads::Threads
        version.lib
)