set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(LIBWIRE_SOURCES
    address.cpp
    internal/endianess.cpp
    tcp/socket.cpp
    tcp/listener.cpp
    error.cpp
    memory_view.cpp
    )

set(LIBWIRE_POSIX_SOURCES
    internal/posix_socket.cpp
    internal/error/posix_dns_category.cpp
    internal/error/posix_system_category.cpp
    posix_dns.cpp
    tcp/posix_options.cpp
    )

set(LIBWIRE_HEADERS
    ../include/libwire.hpp
    ../include/libwire/address.hpp
    ../include/libwire/internal/endianess.hpp
    ../include/libwire/tcp.hpp
    ../include/libwire/tcp/socket.hpp
    ../include/libwire/tcp/options.hpp
    ../include/libwire/tcp/listener.hpp
    ../include/libwire/error.hpp
    ../include/libwire/memory_view.hpp
    ../include/libwire/protocols.hpp
    ../include/libwire/dns.hpp
    )

set(LIBWIRE_POSIX_HEADERS
    ../include/libwire/internal/error/posix_dns_category.hpp
    ../include/libwire/internal/error/posix_system_category.hpp
    ../include/libwire/internal/posix_socket.hpp
    )

if(UNIX)
    set(LIBWIRE_PLATFORM "POSIX")
else()
    message(FATAL "libwire doesn't supports your target platform. :(")
endif()

add_library(libwire ${LIBWIRE_SOURCES} ${LIBWIRE_${LIBWIRE_PLATFORM}_SOURCES})
target_include_directories(libwire PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/>)

install(TARGETS libwire
        EXPORT libwire-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
