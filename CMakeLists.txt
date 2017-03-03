cmake_minimum_required(VERSION 3.7)
project(rackcpp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES rackcpp.cpp rackcpp.h)
add_library(rackcpp SHARED ${SOURCE_FILES})

include_directories(.)

set(HELLO_SOURCE_FILES examples/hello/main.cpp)
add_executable(hello ${HELLO_SOURCE_FILES})
target_link_libraries(hello rackcpp)