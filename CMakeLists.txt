cmake_minimum_required(VERSION 3.6)
project(rackcpp)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)

find_package(LibUV)
set(SOURCE_FILES rackcpp.cpp rackcpp.h http.cpp http.h middlewares/route.cpp middlewares/route.h)
include_directories(${LIBUV_INCLUDE_DIRS})
add_library(rackcpp STATIC ${SOURCE_FILES})
target_link_libraries(rackcpp ${LIBUV_LIBRARIES})

include_directories(${PROJECT_SOURCE_DIR})

set(HELLO_SOURCE_FILES examples/hello/main.cpp)
add_executable(hello ${HELLO_SOURCE_FILES})
target_link_libraries(hello rackcpp)