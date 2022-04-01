cmake_minimum_required(VERSION 3.14)
project(hw2-26)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic -fprofile-arcs -ftest-coverage -fPIC -O0")

# common includes
include_directories(project/common)

# common (vector) library
add_library(lib_common STATIC project/common/common.c)

# static library (sequential)
add_library(lib_static STATIC project/sequential/sequential.c) # no specific includes required

# dynamic library (parallel)
add_library(lib_dynamic SHARED project/parallel/parallel.c)
target_include_directories(lib_dynamic PUBLIC project/parallel)
target_link_libraries(lib_dynamic lib_common)

add_executable(main project/main.c)
target_link_libraries(main lib_static lib_common)

ENABLE_TESTING()

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -fprofile-arcs -ftest-coverage -fPIC -O0")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

add_executable(test_common ${PROJECT_SOURCE_DIR}/project/tests/test_common.cpp)
target_link_libraries(test_common lib_common GTest::gtest_main)

add_executable(test_sequential ${PROJECT_SOURCE_DIR}/project/tests/test_sequential.cpp)
target_link_libraries(test_sequential lib_common lib_static GTest::gtest_main)

add_executable(test_parallel ${PROJECT_SOURCE_DIR}/project/tests/test_parallel.cpp)
target_include_directories(test_parallel PUBLIC ${PROJECT_SOURCE_DIR}/project/parallel)
target_link_libraries(test_parallel lib_common lib_dynamic GTest::gtest_main)