set(LIBWIRE_SOURCES
    impl/endianess.cpp
    impl/posix_socket.cpp
    )

add_library(libwire ${LIBWIRE_SOURCES})
target_include_directories(libwire PUBLIC ${PROJECT_SOURCE_DIR}/include)
