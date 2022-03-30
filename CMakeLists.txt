cmake_minimum_required(VERSION 3.14)
project(hw2-26)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "{CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic -fprofile-arcs -ftest-coverage -O0")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -O0")

