find_package(Doxygen)

if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is required to build documentation.")
endif()

if(DOXYGEN_DOT_FOUND)
    file(TO_NATIVE_PATH "${DOXYGEN_DOT_EXECUTABLE}" DOXYGEN_DOT_EXECUTABLE_)
    set(DOXYGEN_DOT_FOUND_ "YES")
endif()

configure_file(${PROJECT_SOURCE_DIR}/docs/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY)

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/doxygen_output
    COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Generating HTML documentation using Doxygen"
    DEPENDS ${PROJECT_HEADERS} ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in ${PROJECT_SOURCE_DIR}/README.md
    VERBATIM)

add_custom_target(doc ALL
    DEPENDS ${PROJECT_BINARY_DIR}/doxygen_output)

install(DIRECTORY ${PROJECT_BINARY_DIR}/doxygen_output/ DESTINATION share/doc/libwire)
