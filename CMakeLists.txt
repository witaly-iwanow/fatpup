cmake_minimum_required(VERSION 3.14)

# clang++ over g++ any time
set(CMAKE_CXX_COMPILER_NAMES clang++ g++ icpc c++ cxx)
project(fatpup LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS          "Build unit tests"              OFF)
option(BUILD_UCI            "Build UCI executable"          ON)
if (BUILD_TESTS)
    ADD_DEFINITIONS(-DBUILD_TESTS)
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    add_definitions(-DNDEBUG)
endif()

set(FATPUP_HEADERS engines/minimax.h include/fatpup/engine.h include/fatpup/move.h include/fatpup/position.h include/fatpup/square.h)
set(FATPUP_SOURCES engines/engine.cpp engines/minimax.cpp src/move.cpp src/position.cpp src/position_moves.cpp src/position_utils.cpp)

add_library(fatpup STATIC ${FATPUP_HEADERS} ${FATPUP_SOURCES})
target_include_directories(fatpup PUBLIC include)

if (BUILD_UCI)
    add_executable(fatpup_uci engines/uci_main.cpp)
    target_link_libraries(fatpup_uci PRIVATE fatpup)
endif()

get_directory_property(hasParent PARENT_DIRECTORY)
if (hasParent)
    set(FATPUP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
    set(FATPUP_LIBRARIES fatpup PARENT_SCOPE)
endif()

if (BUILD_TESTS)
    add_subdirectory(test)
endif()
