cmake_minimum_required(VERSION 3.7)

add_compile_options(
        -std=c++11
        -Wall
        -D_REENTRANT
        -D_GNU_SOURCE
        -D_XOPEN_SOURCE
        -fPIC
        -m64
        -O0
        -ggdb
)

include_directories(${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/third_party/protobuf/lib)
include_directories(${CMAKE_SOURCE_DIR}/third_party/protobuf/include)

link_libraries(
        protobuf
        pthread
        m
)

add_subdirectory(phxrpc)