CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(hw1-26)
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ENABLE_TESTING()

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_BIN clang-tidy)
    find_program(RUN_CLANG_TIDY_BIN run-clang-tidy.py)

    if(CLANG_TIDY_BIN STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate clang-tidy")
    endif()

    if(RUN_CLANG_TIDY_BIN STREQUAL "RUN_CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate run-clang-tidy.py")
    endif()

    list(APPEND RUN_CLANG_TIDY_BIN_ARGS
            -clang-tidy-binary ${CLANG_TIDY_BIN}
            -header-filter=.*
            -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-misc-noexcept-move-constructor
            )

    add_custom_target(
            tidy
            COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS}
            COMMENT "running clang tidy"
    )

endif()

add_subdirectory(project)