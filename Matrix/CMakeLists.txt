cmake_minimum_required(VERSION 3.16)
project(HW3D)

set(HEADERS_PATH include)
set(SOURCES_PATH src)

add_library(matrix INTERFACE)
target_include_directories(matrix INTERFACE ${HEADERS_PATH})

add_library(task ${SOURCES_PATH}/task.cc)
target_link_libraries(task matrix)

add_executable(main ${SOURCES_PATH}/main.cc)
target_link_libraries(main task)

# Tests
find_package(GTest)
add_executable(utests ${SOURCES_PATH}/utests.cc)
target_link_libraries(utests PRIVATE gtest gtest_main pthread matrix task)