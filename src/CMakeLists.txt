set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(LIBWIRE_SOURCES
    address.cpp
    dns.cpp
    internal/endianess.cpp
    tcp/socket.cpp
    tcp/listener.cpp
    error.cpp)

set(LIBWIRE_POSIX_SOURCES
    internal/posix_socket.cpp
    internal/error/posix_dns_category.cpp
    internal/error/posix_system_category.cpp
    )

if(UNIX)
    set(LIBWIRE_PLATFORM "POSIX")
else()
    message(FATAL "libwire doesn't supports your target platform. :(")
endif()

set(LIBWIRE_SOURCES ${LIBWIRE_SOURCES} ${LIBWIRE_${LIBWIRE_PLATFORM}_SOURCES})

add_library(libwire ${LIBWIRE_SOURCES})
target_include_directories(libwire PUBLIC ${PROJECT_SOURCE_DIR}/include)
