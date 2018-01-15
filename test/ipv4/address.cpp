#include <gtest/gtest.h>
#include <libwire/ipv4/address.hpp>

TEST(IPv4Address, FromString) {
    using namespace libwire::ipv4;

    // Basic test.
    ASSERT_EQ(address("127.0.0.1"), address(127, 0, 0, 1));

    // Multiple zeros is allowed.
    ASSERT_EQ(address("000.00.11.11"), address(0, 0, 11, 11));

#ifdef __cpp_exceptions
    ASSERT_THROW(address("444.4422.22.223"), std::invalid_argument);
    ASSERT_THROW(address("256.0.0.0"), std::invalid_argument);
    ASSERT_THROW(address("0.0.0.0 trailing_characters"), std::invalid_argument);
    ASSERT_THROW(address("0..0.0"), std::invalid_argument);
    ASSERT_THROW(address("leading_characters0.0.0.0"), std::invalid_argument);
#else
    bool success = true;
    address adr1("444.4422.22.223", success);
    ASSERT_FALSE(success) << "444.4422.22.223 accepted as valid IPv4.";
    address adr2("256.0.0.0", success);
    ASSERT_FALSE(success) << "256.0.0.0 accepted as valid IPv4.";
    address adr3("0.0.0.0 trailing_characters", success);
    ASSERT_FALSE(success) << "0.0.0.0 trailing_characters accepted as valid IPv4.";
    address adr4("0..0.0", success);
    ASSERT_FALSE(success) << "0..0.0 accepted as valid IPv4.";
    address adr5("leading_characters0.0.0.0", success);
    ASSERT_FALSE(success) << "leading_characters0.0.0.0 accepted as valid IPv4.";
#endif
}

TEST(IPv4Address, ToString) {
    using namespace libwire::ipv4;

    // Basic test.
    // Also: We should "normalize" output, so only one zero is allowed.
    ASSERT_EQ(address(127, 0, 0, 1).to_string(), "127.0.0.1");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
