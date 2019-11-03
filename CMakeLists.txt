cmake_minimum_required(VERSION 3.1)
project(fatpup)

option(BUILD_TESTS          "Build unit tests"              OFF)
option(BUILD_EXAMPLES       "Build examples"                OFF)
if (BUILD_TESTS)
    ADD_DEFINITIONS(-DBUILD_TESTS)
endif()
if (BUILD_EXAMPLES)
    ADD_DEFINITIONS(-DBUILD_EXAMPLES)
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    add_definitions(-DNDEBUG)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 11)

set(FATPUP_HEADERS engines/minimax.h include/fatpup/engine.h include/fatpup/move.h include/fatpup/position.h include/fatpup/square.h)
set(FATPUP_SOURCES engines/engine.cpp engines/minimax.cpp src/move.cpp src/position.cpp src/position_moves.cpp src/position_utils.cpp)

add_library(fatpup STATIC ${FATPUP_HEADERS} ${FATPUP_SOURCES})
target_include_directories(fatpup PUBLIC include)

get_directory_property(hasParent PARENT_DIRECTORY)
if (hasParent)
    set(FATPUP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
    set(FATPUP_LIBRARIES fatpup PARENT_SCOPE)
endif()

if (BUILD_TESTS)
    add_subdirectory(test)
endif()

if (BUILD_EXAMPLES)
    # coming up soon
    add_subdirectory(examples)
endif()
