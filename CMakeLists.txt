cmake_minimum_required(VERSION 3.2)
project(rackcpp)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)

find_package(LibUV)
set(SOURCE_FILES
        http/http.cpp http/response.cpp http/client.cpp
        http/parser/parser.cpp http/parser/start_line_parser.cpp http/parser/abstract_parser.cpp
        http/parser/header_parser.cpp http/parser/sized_body_parser.cpp http/parser/final_parser.cpp
        http/parser/queries_parser.cpp
        middlewares/middleware.cpp middlewares/route.cpp
        utils/logger.cpp utils/buffer.cpp utils/string.cpp utils/protocol_helper.cpp
        http/request/request.cpp http/request/header.cpp)
include_directories(${LIBUV_INCLUDE_DIRS})
add_library(rackcpp STATIC ${SOURCE_FILES})
target_link_libraries(rackcpp ${LIBUV_LIBRARIES})

include_directories(${PROJECT_SOURCE_DIR})

set(HELLO_SOURCE_FILES examples/hello/main.cpp examples/hello/middlewares/hello.cpp)
add_executable(hello ${HELLO_SOURCE_FILES})
target_link_libraries(hello rackcpp)

add_executable(buffer_test test/buffer_test.cpp)
target_link_libraries(buffer_test rackcpp)
