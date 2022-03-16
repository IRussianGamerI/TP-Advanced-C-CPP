include_directories(${PROJECT_SOURCE_DIR}/project/include)

add_executable(main src/main.c src/server.c src/group_of_networks.c)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test tst/test.cpp)
