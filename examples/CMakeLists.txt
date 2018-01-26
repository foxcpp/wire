macro(libwire_example name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} libwire)
    add_dependencies(examples ${name})
endmacro()

add_custom_target(examples)

libwire_example(echo-client echo_client.cpp)
libwire_example(echo-server echo_server.cpp)
libwire_example(dns-query   dns_query.cpp)

