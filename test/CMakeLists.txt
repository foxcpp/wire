enable_testing()

macro(libwire_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} libwire gtest)
    add_test(NAME ${name} COMMAND ${name})
endmacro()

libwire_test(endianess      internal/endianess.cpp)
libwire_test(address        address.cpp)
