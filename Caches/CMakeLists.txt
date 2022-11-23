cmake_minimum_required(VERSION 3.16)
project(Caches)

set(HEADERS_PATH include)
set(SOURCES_PATH src)
set(TESTS_PATH   tests)
set(TESTERS_PATH ${SOURCES_PATH}/testers)

add_library(libcaches INTERFACE)
target_include_directories(libcaches INTERFACE ${HEADERS_PATH})

# Drivers
add_executable(LRU ${SOURCES_PATH}/LRU.cc)
target_link_libraries(LRU libcaches)

add_executable(LFU ${SOURCES_PATH}/LFU.cc)
target_link_libraries(LFU libcaches)

add_executable(ideal ${SOURCES_PATH}/ideal.cc)
target_link_libraries(ideal libcaches)

# Testers
add_executable(LRUtest ${TESTERS_PATH}/LRUtest.cc)
target_link_libraries(LRUtest libcaches)

add_executable(LFUtest ${TESTERS_PATH}/LFUtest.cc)
target_link_libraries(LFUtest libcaches)

add_executable(idealtest ${TESTERS_PATH}/idealtest.cc)
target_link_libraries(idealtest libcaches)

# Testing
enable_testing()

function(testgen tester testfile)
  file(READ "${testfile}" testcontent)
  message("Loaded test ${tester}: ${testfile}")

  set(args ${testcontent})
  separate_arguments(args UNIX_COMMAND ${args})

  add_test(NAME "${tester}${testfile}" COMMAND ${tester} ${args})
endfunction()

function(load_tests tester testfolder)
  file(GLOB testfiles "${TESTS_PATH}/${testfolder}/*")
  foreach(testfile ${testfiles})
    testgen(${tester} ${testfile})
  endforeach()
endfunction()

# Loading tests

load_tests(LRUtest LRU)
load_tests(LFUtest LFU)
load_tests(idealtest ideal)