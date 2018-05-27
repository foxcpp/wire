file(GLOB_RECURSE TEST_SOURCES *.cpp)
add_executable(libwire-tests ${TEST_SOURCES})
target_link_libraries(libwire-tests libwire gtest)
add_test(NAME libwire COMMAND libwire-tests)

