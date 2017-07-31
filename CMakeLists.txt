cmake_minimum_required(VERSION 3.2)
project(rackcpp)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(LibUV REQUIRED)
find_package(spdlog REQUIRED)

set(SOURCE_FILES
        http/http.cpp http/response.cpp http/client.cpp
        http/parser/parser.cpp http/parser/start_line_parser.cpp http/parser/abstract_parser.cpp
        http/parser/header_parser.cpp http/parser/sized_body_parser.cpp http/parser/final_parser.cpp
        http/parser/queries_parser.cpp http/parser/body_form_parser.cpp
        http/parser/parser_helper.cpp http/parser/multipart_form_parser.cpp
        middlewares/middleware.hpp middlewares/simple_middleware.cpp middlewares/route.cpp
        utils/logger.cpp utils/buffer.cpp utils/protocol_helper.cpp utils/string_utils.cpp
        utils/case_insensitive_map.hpp
        http/request/request.cpp http/request/header.hpp http/request/form_data.cpp
        http/request/header_fields/content_type.cpp http/errors.hpp
        ecp/template.hpp
        constants.hpp)

include_directories(${LIBUV_INCLUDE_DIRS})
include_directories(${SPDLOG_INCLUDE_DIRS})

set(RACKCPP_LIBRARIES ${LIBUV_LIBRARIES})

if (NOT APPLE AND NOT MSVC)
    set(RACKCPP_LIBRARIES ${RACKCPP_LIBRARIES} pthread)
endif ()

add_library(rackcpp STATIC ${SOURCE_FILES})
target_link_libraries(rackcpp ${RACKCPP_LIBRARIES})

add_executable(ecpc ecp/compiler.cpp)

include_directories(${PROJECT_SOURCE_DIR})

set(HELLO_SOURCE_FILES examples/hello/main.cpp)
add_executable(hello ${HELLO_SOURCE_FILES})
target_link_libraries(hello rackcpp)

add_executable(buffer_test test/buffer_test.cpp)
target_link_libraries(buffer_test rackcpp)
