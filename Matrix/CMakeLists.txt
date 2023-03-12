cmake_minimum_required(VERSION 3.16)
project(HW3D)

set(HEADERS_PATH include)
set(SOURCES_PATH src)

add_library(matrix INTERFACE)
target_include_directories(matrix INTERFACE ${HEADERS_PATH})

add_executable(main ${SOURCES_PATH}/main.cc)
target_link_libraries(main matrix)

# Tests
add_subdirectory(../googletest bin)
add_executable(utests ${SOURCES_PATH}/utests.cc)
target_link_libraries(utests PRIVATE GTest::gtest_main matrix)