## Install configuration for libwire
#
# Defines following variables:
# LIBWIRE_FOUND        - whether libwire found
# LIBWIRE_INCLUDE_DIRS - the libwire include directories
# LIBWIRE_LIBRARIES    - link to these libraries to use libwire
#
# Also "libwire" imported target created.

get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

set(LIBWIRE_FOUND ON)

cmake_policy(PUSH)
cmake_policy(SET CMP0003 NEW)

if(NOT TARGET libwire AND NOT LIBWIRE_BINARY_DIR)
    include(${SELF_DIR}/libwire-targets.cmake)
endif()

get_property(LIGWIRE_INCLUDE_DIRS
        TARGET libwire
        PROPERTY INTERFACE_INCLUDE_DIRECTORIES
        )

set(LIBWIRE_LIBRARIES libwire)

cmake_policy(POP)
