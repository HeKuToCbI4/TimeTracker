project(proto VERSION 1.0 LANGUAGES CXX C)
INCLUDE(FindProtobuf)
# packages
FIND_PACKAGE(Protobuf REQUIRED)
FIND_PACKAGE(gRPC CONFIG REQUIRED)
# directories
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
# proto files
#file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
set(PROTO_FILES FrameInfoService.proto FrameInfo.proto)
# found proto files
message(${PROTO_FILES})
add_library(proto ${PROTO_FILES})
# add libs
target_link_libraries(proto
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)
# dunno why but
target_include_directories(proto
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
)
# get plugin location
GET_TARGET_PROPERTY(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
# generate cpp files
# PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROTO_FILES} PLUGIN protoc-gen-grpc=${GRPC_CPP_PLUGIN_PATH})
# compile the message types
protobuf_generate(TARGET proto LANGUAGE cpp)

# compile the GRPC services
protobuf_generate(
        TARGET
        proto
        LANGUAGE
        grpc
        GENERATE_EXTENSIONS
        .grpc.pb.h
        .grpc.pb.cc
        PLUGIN
        "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)