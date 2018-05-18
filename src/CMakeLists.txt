set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

file(GLOB LIBWIRE_PORTABLE_SOURCES
        *.cpp
        tcp/*.cpp
        internal/*.cpp)
file(GLOB LIBWIRE_POSIX_SOURCES
        posix/*.cpp)
file(GLOB LIBWIRE_PORTABLE_HEADERS
        ../include/*.hpp
        ../include/libwire/*.hpp
        ../include/libwire/internal/*.hpp
        ../include/libwire/tcp/*.hpp
        ../include/tcp/*.hpp)
file(GLOB LIBWIRE_POSIX_HEADERS
        ../include/libwire/posix/*.hpp)

set(LIBWIRE_SOURCES ${LIBWIRE_PORTABLE_SOURCES})
set(LIBWIRE_HEADERS ${LIBWIRE_PORTABLE_HEADERS})
if(UNIX)
    message(STATUS "libwire: building for generic POSIX platform")
    set(LIBWIRE_SOURCES ${LIBWIRE_SOURCES} ${LIBWIRE_POSIX_SOURCES})
    set(LIBWIRE_HEADERS ${LIBWIRE_HEADERS} ${LIBWIRE_POSIX_HEADERS})
else()
    message(FATAL "libwire doesn't supports your platform yet. :(")
endif()

set(LIBWIRE_ALL_SOURCES ${LIBWIRE_SOURCES} PARENT_SCOPE)
set(LIBWIRE_ALL_HEADERS ${LIBWIRE_HEADERS} PARENT_SCOPE)

add_library(libwire ${LIBWIRE_SOURCES} ${LIBWIRE_HEADERS})
target_include_directories(libwire PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/>)

include(GNUInstallDirs)

install(TARGETS libwire
        EXPORT libwire-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
