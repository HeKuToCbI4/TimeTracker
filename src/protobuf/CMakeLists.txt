# This is used to build protobuf from sources.
cmake_minimum_required(VERSION 3.23)
project(TimeTracker)
INCLUDE(FindProtobuf)
# packages
message("Looking for protobuf")
FIND_PACKAGE(Protobuf REQUIRED)
message("Looking for grpc")
FIND_PACKAGE(gRPC CONFIG REQUIRED)

message("Protobuf VERSION: ${Protobuf_VERSION}")
message("Protobuf INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")
message("Protobuf LIBRARY_DIRS: ${Protobuf_LIBRARY_DIRS}")
message("Protobuf LIBRARIES: ${Protobuf_LIBRARIES}")

# directories
INCLUDE_DIRECTORIES(${Protobuf_INCLUDE_DIRS})
# proto files
set(PROTO_FILES
        ${CMAKE_CURRENT_LIST_DIR}/proto/FrameInfoService.proto
        ${CMAKE_CURRENT_LIST_DIR}/proto/FrameInfo.proto
)
set(PROTO_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
# found proto files
message(${PROTO_FILES})
add_library(proto ${PROTO_FILES})
add_library(TimeTracker::proto ALIAS proto)
# add libs
target_link_libraries(proto
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)

target_include_directories(
        proto
        PUBLIC $<BUILD_INTERFACE:${PROTO_BINARY_DIR}>
)

# get plugin location
GET_TARGET_PROPERTY(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
# compile the message types
protobuf_generate(
    TARGET proto
    IMPORT_DIRS ${CMAKE_CURRENT_LIST_DIR}/proto
    LANGUAGE cpp
    PROTOC_OUT_DIR ${PROTO_BINARY_DIR}
)

# compile the GRPC services
protobuf_generate(
        TARGET proto
        IMPORT_DIRS ${CMAKE_CURRENT_LIST_DIR}/proto
        LANGUAGE grpc
        GENERATE_EXTENSIONS
        .grpc.pb.h
        .grpc.pb.cc
        PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
        PROTOC_OUT_DIR ${PROTO_BINARY_DIR}
)
