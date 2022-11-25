cmake_minimum_required(VERSION 3.16)
project(HW3D)

set(HEADERS_PATH include)
set(SOURCES_PATH src)

add_library(lib3D INTERFACE)
target_include_directories(lib3D INTERFACE ${HEADERS_PATH})

add_executable(main ${SOURCES_PATH}/main.cc)
target_link_libraries(main lib3D)

# Tests
find_package(GTest)
add_executable(utests ${SOURCES_PATH}/utests.cc)
target_link_libraries(utests PRIVATE gtest gtest_main pthread lib3D)
