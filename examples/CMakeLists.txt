macro(libwire_example name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} libwire)
endmacro()

libwire_example(echo-client echo_client.cpp)
libwire_example(echo-server echo_server.cpp)

add_custom_target(examples)
add_dependencies(examples echo-client echo-server)
