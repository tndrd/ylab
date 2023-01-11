cmake_minimum_required(VERSION 3.16)
project(HWCache)

set(HEADERS_PATH include)
set(SOURCES_PATH src)

add_library(libcaches INTERFACE)
target_include_directories(libcaches INTERFACE ${HEADERS_PATH})

# Drivers
add_executable(LRU ${SOURCES_PATH}/LRU.cc)
target_link_libraries(LRU libcaches)

add_executable(LFU ${SOURCES_PATH}/LFU.cc)
target_link_libraries(LFU libcaches)

add_executable(ideal ${SOURCES_PATH}/ideal.cc)
target_link_libraries(ideal libcaches)

# Testing

add_executable(test ${SOURCES_PATH}/test.cc)
target_link_libraries(test PRIVATE gtest gtest_main pthread libcaches)