cmake_minimum_required(VERSION 3.16)
project(HW3D)

set(HEADERS_PATH include)
set(SOURCES_PATH src)


set(CMAKE_CXX_FLAGS "-O2 -g -Wall -Wextra")

add_library(lib3D INTERFACE)
target_include_directories(lib3D INTERFACE ${HEADERS_PATH})

add_library(triangle_intersections ${SOURCES_PATH}/triangle_intersections.cc)
target_link_libraries(triangle_intersections lib3D)

add_library(task ${SOURCES_PATH}/task.cc)
target_link_libraries(task triangle_intersections)

add_executable(main ${SOURCES_PATH}/main.cc)
target_link_libraries(main task)

# Tests
find_package(GTest)
add_executable(utests ${SOURCES_PATH}/utests.cc)
target_link_libraries(utests PRIVATE gtest gtest_main pthread triangle_intersections task)
