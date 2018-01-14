set(LIBWIRE_SOURCES
    internal/endianess.cpp
    tcp/socket.cpp
    )

set(LIBWIRE_POSIX_SOURCES
    internal/posix_socket.cpp
    )

if(UNIX)
    set(LIBWIRE_PLATFORM "POSIX")
else()
    message(FATAL "libwire doesn't supports your target platform. :(")
endif()

set(LIBWIRE_SOURCES ${LIBWIRE_SOURCES} ${LIBWIRE_${LIBWIRE_PLATFORM}_SOURCES})

add_library(libwire ${LIBWIRE_SOURCES})
target_include_directories(libwire PUBLIC ${PROJECT_SOURCE_DIR}/include)
