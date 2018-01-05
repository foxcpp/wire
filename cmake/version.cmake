set(LIBWIRE_VERSION_OVERRIDE "0.0.0" CACHE STRING "libwire version override")

if(NOT (LIBWIRE_VERSION_OVERRIDE STREQUAL "0.0.0"))
    message(STATUS "Skipping version detection using Git tags...")
    set(LIBWIRE_DETECTED_VERSION "${LIBWIRE_VERSION_OVERRIDE}")
    set(LIBWIRE_SHORT_VERSION "${LIBWIRE_VERSION_OVERRIDE}")
else()
    find_package(Git QUIET)
    if(Git_FOUND)
        execute_process(
                COMMAND           "${GIT_EXECUTABLE} status"
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                RESULT_VARIABLE   git_status_result
                OUTPUT_VARIABLE   git_status_output
                ERROR_VARIABLE    git_status_error
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_STRIP_TRAILING_WHITESPACE)

        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version_from_git.cmake)
            version_from_git(GIT_EXECUTABLE ${GIT_EXECUTABLE} INCLUDE_HASH ON)

            set(LIBWIRE_DETECTED_VERSION "${SEMVER}")
            set(LIBWIRE_SHORT_VERSION "${VERSION}")
        else()
            message(WARNING "Setting version to default string, because building outside of Git repository, please specify version by setting LIBWIRE_VERSION CMake variable.")
            set(LIBWIRE_DETECTED_VERSION "0.0.0")
            set(LIBWIRE_SHORT_VERSION "0.0.0")
        endif()
    else()
        message(WARNING "Setting version to default string, because Git executable not found, please specify version by setting LIBWIRE_VERSION CMake variable.")
        set(LIBWIRE_DETECTED_VERSION "0.0.0")
        set(LIBWIRE_SHORT_VERSION "0.0.0")
    endif()
endif()
