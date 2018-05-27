cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)

include(cmake/version.cmake)

project(LIBWIRE
    VERSION ${LIBWIRE_SHORT_VERSION}
    LANGUAGES CXX)

message(STATUS "libwire: detected version: ${LIBWIRE_DETECTED_VERSION}")

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
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "(Clang|AppleClang)")
        message(FATAL_ERROR "Clang is required for LIBWIRE_WSTRICT.")
    endif()

    target_compile_options(libwire PUBLIC -Wall -Wextra -pedantic -Wdocumentation
        -Wconversion -Wdeprecated -Wformat -Weffc++)
endif()

#------------------------------------------------------------------------------
# Tests

option(LIBWIRE_TESTS "Enable tests (requires Google Test submodule)" OFF)

if(LIBWIRE_TESTS)
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/gtest/CMakeLists.txt)
        message(FATAL_ERROR "Google Test submodule is missing.")
    endif()

    add_subdirectory(external/gtest/googletest EXCLUDE_FROM_ALL)

    add_subdirectory(test/)
elseif(EXISTS ${PROJECT_SOURCE_DIR}/external/gtest/CMakeLists.txt)
    add_subdirectory(external/gtest/googletest EXCLUDE_FROM_ALL)
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

#------------------------------------------------------------------------------
# Library installation

include(GNUInstallDirs)

install(FILES include/libwire.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

install(DIRECTORY include/libwire
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

export(TARGETS libwire
        FILE "${PROJECT_BINARY_DIR}/libwire-targets.cmake"
        )

export(PACKAGE libwire)

install(EXPORT libwire-targets
        DESTINATION lib/cmake/libwire
        )

configure_file(${PROJECT_SOURCE_DIR}/cmake/libwire-config-version.cmake.in
        "${PROJECT_BINARY_DIR}/libwire-config-version.cmake" @ONLY)

install(FILES
        "${PROJECT_SOURCE_DIR}/cmake/libwire-config.cmake"
        "${PROJECT_BINARY_DIR}/libwire-config-version.cmake"
        DESTINATION lib/cmake/libwire)

#-----------------------------------------------------------------------------
# Linters/style checkers

find_program(clangtidy_path NAMES clang-tidy)
if(clangtidy_path)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_custom_target(tidy
        COMMAND "${clangtidy_path}" -p "${PROJECT_BINARY_DIR}" ${LIBWIRE_ALL_SOURCES}
        COMMENT "Running clang-tidy tool"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/"
        SOURCES ${PROJECT_SOURCE_DIR}/.clang-tidy
        )
endif()

find_program(clangformat_path NAMES clang-format)
if(clangformat_path)
    add_custom_target(format
        COMMAND "${clangformat_path}" -i -style=file ${LIBWIRE_ALL_SOURCES} ${LIBWIRE_ALL_HEADERS}
        COMMENT "Running clang-format tool"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/src"
        SOURCES ${PROJECT_SOURCE_DIR}/.clang-format
        )
endif()

#------------------------------------------------------------------------------
# Coverage generation

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(DEFAULT_LIBWIRE_COVERAGE ON)
elseif()
    set(DEFAULT_LIBWIRE_COVERAGE OFF)
endif()

option(LIBWIRE_COVERAGE "Enable coverage build type option" ${DEFAULT_LIBWIRE_COVERAGE})

if(LIBWIRE_COVERAGE)
    include(cmake/code_coverage.cmake)

    setup_target_for_coverage(
        check
        "libwire-tests --gtest_filter=-*Ipv6*:-*IPv6*"
        coverage
    )
else()
    add_custom_target(
        check
        COMMAND libwire-tests --gtest_filter=-*Ipv6*:-*IPv6*
        COMMENT "Running libwire tests"
        )
endif()
