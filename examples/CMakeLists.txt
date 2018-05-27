macro(libwire_example name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} libwire)
    add_dependencies(examples ${name})
endmacro()

add_custom_target(examples)

libwire_example(tcp-echo-client tcp_echo_client.cpp)
libwire_example(tcp-echo-server tcp_echo_server.cpp)
libwire_example(udp-echo-client udp_echo_client.cpp)
libwire_example(udp-echo-server udp_echo_server.cpp)
libwire_example(dns-query       dns_query.cpp)

