cmake_minimum_required(VERSION 3.2)
project(hane)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(LibUV REQUIRED)
find_package(spdlog REQUIRED)

set(SOURCE_FILES
        src/http/http.cpp src/http/response.cpp src/http/client.cpp
        src/http/parser/parser.cpp src/http/parser/start_line_parser.cpp src/http/parser/abstract_parser.cpp
        src/http/parser/header_parser.cpp src/http/parser/sized_body_parser.cpp src/http/parser/final_parser.cpp
        src/http/parser/queries_parser.cpp src/http/parser/body_form_parser.cpp
        src/http/parser/parser_helper.cpp src/http/parser/multipart_form_parser.cpp
        src/middlewares/middleware.hpp src/middlewares/simple_middleware.cpp src/middlewares/route.cpp
        src/utils/logger.cpp src/utils/buffer.cpp src/utils/protocol_helper.cpp src/utils/string_utils.cpp
        src/utils/case_insensitive_map.hpp
        src/http/request/request.cpp src/http/request/header.hpp src/http/request/form_data.cpp
        src/http/request/header_fields/content_type.cpp src/http/errors.hpp
        src/ecp/template.hpp
        src/constants.hpp)

include_directories(${LIBUV_INCLUDE_DIRS})
include_directories(${SPDLOG_INCLUDE_DIRS})

set(HANE_LIBRARIES ${LIBUV_LIBRARIES})

if (NOT APPLE AND NOT MSVC)
    set(HANE_LIBRARIES ${HANE_LIBRARIES} pthread)
endif ()

add_library(hane STATIC ${SOURCE_FILES})
target_link_libraries(hane ${HANE_LIBRARIES})

add_executable(ecpc src/ecp/compiler.cpp)
target_link_libraries(ecpc)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(HELLO_SOURCE_FILES examples/hello/main.cpp)
file(GLOB HELLO_TEMPLATE_FILES "examples/hello/templates/*.cpp")
add_executable(hello ${HELLO_SOURCE_FILES} ${HELLO_TEMPLATE_FILES})
target_link_libraries(hello hane)

add_executable(buffer_test test/buffer_test.cpp)
target_link_libraries(buffer_test hane)

install(TARGETS hane DESTINATION lib)
install(TARGETS ecpc DESTINATION bin)
install(DIRECTORY src/ DESTINATION include/hane
        FILES_MATCHING PATTERN "*.hpp")