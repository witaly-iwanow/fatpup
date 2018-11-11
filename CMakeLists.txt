CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(fatpup)

IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
    ADD_DEFINITIONS(-DNDEBUG)
ENDIF()

SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_STANDARD 11)

SET(FATPUP_HEADERS capture_solver.h checkmate_solver.h move.h performance_tests.h position.h possible_moves_tests.h rang.h solver.h square.h)
SET(FATPUP_SOURCES capture_solver.cpp checkmate_solver.cpp move.cpp performance_tests.cpp position.cpp position_moves.cpp position_utils.cpp possible_moves_tests.cpp solver.cpp square.cpp)

ADD_EXECUTABLE(fatpup ${FATPUP_HEADERS} ${FATPUP_SOURCES} fatpup.cpp)
