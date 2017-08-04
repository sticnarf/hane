cmake_minimum_required(VERSION 3.2)
project(hello)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_library(LIBUV_LIBRARY NAMES uv)
find_library(HANE_LIBRARY NAMES hane)
find_path(HANE_INCLUDE_DIRS hane.hpp)
include_directories(${HANE_INCLUDE_DIRS})

execute_process(COMMAND ${PROJECT_SOURCE_DIR}/ecpgen WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

set(HELLO_SOURCE_FILES main.cpp)
file(GLOB HELLO_TEMPLATE_FILES "templates/*.cpp")
add_executable(hello ${HELLO_SOURCE_FILES} ${HELLO_TEMPLATE_FILES})
target_link_libraries(hello hane uv)
install(TARGETS hello DESTINATION bin)