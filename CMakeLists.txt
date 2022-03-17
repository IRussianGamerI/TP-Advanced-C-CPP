CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
PROJECT(hw1-26)
SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include_directories(${PROJECT_SOURCE_DIR}/project/include)

add_library(server STATIC ${PROJECT_SOURCE_DIR}/project/src/server.c
        ${PROJECT_SOURCE_DIR}/project/src/group_of_networks.c)

add_executable(main ${PROJECT_SOURCE_DIR}/project/src/main.c)
target_link_libraries(main server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov")

ENABLE_TESTING()

file(GLOB SOURCES *.cpp)

find_package(GTest REQUIRED)

add_executable(test_lib ${PROJECT_SOURCE_DIR}/project/tst/test.cpp)
target_include_directories(test_lib PUBLIC ${TEST_SERVER_INCLUDE_DIRS})
target_link_libraries(test_lib server ${TEST_SERVER_LIBRARIES} GTest::gtest_main)