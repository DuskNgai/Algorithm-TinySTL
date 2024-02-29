cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "Algorithms-TinySTL")
project(${PROJECT_NAME} LANGUAGES C CXX)

# Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS")
endif()

# Building type
set(CMAKE_BUILD_TYPE "Release")

# Include cmake scripts
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake")

# gtest
enable_testing()

list(APPEND ALGORITHM_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/dependency/gtest/googletest"
    "${CMAKE_CURRENT_SOURCE_DIR}/Algorithm"
    "${CMAKE_CURRENT_SOURCE_DIR}/TinySTL"
)

list(APPEND ALGORITHM_LIBRARIES
    gtest
    gtest_main
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
