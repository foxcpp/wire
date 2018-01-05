cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)

include(cmake/version.cmake)

project(LIBWIRE
    VERSION ${LIBWIRE_SHORT_VERSION}
    LANGUAGES CXX)

message(STATUS "Configuring libwire ${LIBWIRE_DETECTED_VERSION}")

#------------------------------------------------------------------------------
# C++ standard

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#------------------------------------------------------------------------------
# Threads

find_package(Threads REQUIRED)

#------------------------------------------------------------------------------
# Library

add_subdirectory(src/)

#------------------------------------------------------------------------------
# Tests

option(LIBWIRE_TESTS "Enable tests" OFF)

if(LIBWIRE_TESTS)
    add_subdirectory(external/gtest/googletest EXCLUDE_FROM_ALL)
    enable_testing()
    add_subdirectory(test)
endif()

#------------------------------------------------------------------------------
# Examples

add_subdirectory(examples/ EXCLUDE_FROM_ALL)

#------------------------------------------------------------------------------
# Documentation (Doxygen)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    add_subdirectory(docs/ EXCLUDE_FROM_ALL)
endif()

