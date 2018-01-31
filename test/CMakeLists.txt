enable_testing()

macro(libwire_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} libwire gtest)
    add_test(NAME ${name} COMMAND ${name})
endmacro()

libwire_test(libwire-tests
    internal/endianess.cpp
    address.cpp
    memory_view.cpp
    dns.cpp
    main.cpp
    tcp/socket.cpp
    )

add_custom_target(
    check
    COMMAND libwire-tests
    COMMENT "Running libwire tests"
    )
