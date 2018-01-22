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

option(LIBWIRE_WSTRICT "Extra warnings (Clang-only)" OFF)

if(LIBWIRE_WSTRICT)
    target_compile_options(libwire PUBLIC -Wall -Wextra -pedantic -Wdocumentation
        -Wconversion -Wdeprecated -Wformat -Weffc++)
endif()

#------------------------------------------------------------------------------
# Tests

option(LIBWIRE_TESTS "Enable tests" OFF)

add_subdirectory(external/gtest/googletest EXCLUDE_FROM_ALL)

if(LIBWIRE_TESTS)
    add_subdirectory(test/)
else()
    add_subdirectory(test/ EXCLUDE_FROM_ALL)
endif()

#------------------------------------------------------------------------------
# Examples

option(LIBWIRE_EXAMPLES "Build examples" OFF)

if(LIBWIRE_EXAMPLES)
    add_subdirectory(examples/)
else()
    add_subdirectory(examples/ EXCLUDE_FROM_ALL)
endif()

#------------------------------------------------------------------------------
# Documentation (Doxygen)

find_package(Doxygen)

option(LIBWIRE_DOCS "Build documentation (requires Doxygen)" OFF)

if(LIBWIRE_DOCS)
    if(NOT DOXYGEN_FOUND)
        message(FATAL "Doxygen is required to build documentation.")
    endif()
    add_subdirectory(docs/)
else()
    if(DOXYGEN_FOUND)
        add_subdirectory(docs/ EXCLUDE_FROM_ALL)
    endif()
endif()
