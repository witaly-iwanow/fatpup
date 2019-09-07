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

SET(FATPUP_HEADERS include/fatpup/move.h include/fatpup/position.h include/fatpup/square.h)
SET(FATPUP_SOURCES src/move.cpp src/position.cpp src/position_moves.cpp src/position_utils.cpp)

ADD_LIBRARY(fatpup STATIC ${FATPUP_HEADERS} ${FATPUP_SOURCES})

GET_DIRECTORY_PROPERTY(hasParent PARENT_DIRECTORY)
IF (hasParent)
    SET(FATPUP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
    SET(FATPUP_LIBRARIES fatpup PARENT_SCOPE)
ENDIF()

IF (BUILD_CLI)
    SET(FATPUP_CLI_HEADERS test/capture_solver.h test/checkmate_solver.h test/color_scheme.h test/performance_tests.h test/possible_moves_tests.h test/rang.h test/solver.h test/utils.h)
    SET(FATPUP_CLI_SOURCES test/capture_solver.cpp test/checkmate_solver.cpp test/performance_tests.cpp test/possible_moves_tests.cpp test/solver.cpp test/utils.cpp)

    ADD_EXECUTABLE(fatpup-cli ${FATPUP_CLI_HEADERS} ${FATPUP_CLI_SOURCES} test/fatpup-cli.cpp)
    TARGET_LINK_LIBRARIES(fatpup-cli fatpup)
ENDIF()
