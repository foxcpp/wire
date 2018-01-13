enable_testing()

add_executable(util_tests
               internal/endianess.cpp)
target_link_libraries(util_tests libwire gtest)
add_test(NAME util_tests
         COMMAND util_tests)
