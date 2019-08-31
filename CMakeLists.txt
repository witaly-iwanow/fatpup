CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(fatpup)

OPTION(BUILD_CLI "Build CLI test app"           OFF)

IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
    ADD_DEFINITIONS(-DNDEBUG)
ENDIF()

SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(include)

SET(FATPUP_HEADERS include/color_scheme.h include/move.h include/position.h include/rang.h include/square.h)
SET(FATPUP_SOURCES src/move.cpp src/position.cpp src/position_moves.cpp src/position_utils.cpp)

ADD_LIBRARY(fatpup STATIC ${FATPUP_HEADERS} ${FATPUP_SOURCES})

IF (BUILD_CLI)
    SET(FATPUP_CLI_HEADERS test/capture_solver.h test/checkmate_solver.h test/performance_tests.h test/possible_moves_tests.h test/solver.h)
    SET(FATPUP_CLI_SOURCES test/capture_solver.cpp test/checkmate_solver.cpp test/performance_tests.cpp test/possible_moves_tests.cpp test/solver.cpp)

    ADD_EXECUTABLE(fatpup-cli ${FATPUP_CLI_HEADERS} ${FATPUP_CLI_SOURCES} test/fatpup-cli.cpp)
    TARGET_LINK_LIBRARIES(fatpup-cli fatpup)
ENDIF()
