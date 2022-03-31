cmake_minimum_required(VERSION 3.14)
project(hw2-26)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic -fprofile-arcs -ftest-coverage -O0")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -fprofile-arcs -ftest-coverage -O0")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

# common includes
include_directories(common)

# common (vector) library
add_library(common STATIC ${PROJECT_SOURCE_DIR}/common/common.c)

# static library (sequential)
add_library(lib_static STATIC ${PROJECT_SOURCE_DIR}/sequential/sequential.c) # no specific includes required

# dynamic library (parallel)
add_library(lib_dynamic SHARED parallel/parallel.c)
target_include_directories(lib_dynamic PUBLIC parallel)
target_link_libraries(lib_dynamic common)

add_executable(main main.c)
target_link_libraries(main lib_static common)