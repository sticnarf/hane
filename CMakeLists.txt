cmake_minimum_required(VERSION 3.2)
project(hane)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(LibUV REQUIRED)
find_package(spdlog REQUIRED)

include_directories(${LIBUV_INCLUDE_DIRS})
include_directories(${SPDLOG_INCLUDE_DIRS})

add_executable(ecpc src/ecp/compiler.cpp)
target_link_libraries(ecpc)

set(SOURCE_FILES
        src/http/http.cpp src/http/client.cpp src/http/errors.hpp
        src/http/parser/parser.cpp src/http/parser/start_line_parser.cpp src/http/parser/abstract_parser.cpp
        src/http/parser/header_parser.cpp src/http/parser/sized_body_parser.cpp src/http/parser/final_parser.cpp
        src/http/parser/url_queries_parser.cpp src/http/parser/body_form_parser.cpp src/http/parser/chunked_body_parser.cpp
        src/http/parser/parser_helper.cpp src/http/parser/multipart_form_parser.cpp
        src/middlewares/middleware.hpp src/middlewares/simple_middleware.cpp src/middlewares/route.cpp
        src/middlewares/static_middleware.cpp src/middlewares/session_middleware.cpp
        src/utils/logger.cpp src/utils/buffer.cpp src/utils/protocol_helper.cpp src/utils/string_utils.cpp
        src/utils/case_insensitive_map.hpp
        src/http/request/request.cpp src/http/header/header.hpp src/http/request/form_data.cpp
        src/http/header/content_type.cpp src/http/header/cookies.cpp
        src/http/response/response.cpp src/http/response/chunked_response.cpp
        src/http/session/session_base.cpp
        src/ecp/template.hpp
        src/constants.hpp src/hane.hpp src/middlewares/lambda_middleware.cpp src/middlewares/lambda_middleware.hpp)

set(HANE_LIBRARIES ${LIBUV_LIBRARIES})

add_library(hane SHARED ${SOURCE_FILES})
target_link_libraries(hane ${HANE_LIBRARIES})

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(buffer_test test/buffer_test.cpp)
target_link_libraries(buffer_test hane)

install(TARGETS hane DESTINATION lib)
install(TARGETS ecpc DESTINATION bin)
install(DIRECTORY src/ DESTINATION include/hane
        FILES_MATCHING PATTERN "*.hpp")