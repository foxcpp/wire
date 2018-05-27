file(GLOB_RECURSE TEST_SOURCES *.cpp)
add_executable(libwire-tests ${TEST_SOURCES})
target_link_libraries(libwire-tests libwire gtest)

option(LIBWIRE_TESTS_NOIPV6 "Disable IPv6-related tests" OFF)
if (LIBWIRE_TESTS_NOIPV6)
    message(STATUS "libwire: IPv6 tests are disabled")
    add_test(NAME libwire COMMAND "libwire-tests --gtest_filter=-*Ipv6*:-*IPv6*")
else()
    add_test(NAME libwire COMMAND libwire-tests)
endif()

