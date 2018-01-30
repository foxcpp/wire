#include "gtest.hpp"
#include <libwire/address.hpp>

TEST(IPv4Address, VersionInitialization) {
    using namespace libwire;

    address adr1({127, 0, 0, 1});
    ASSERT_EQ(adr1.version, ip::v4);
}

TEST(IPv4Address, FromString) {
    using namespace libwire;

    // Basic test.
    ASSERT_EQ(address("127.0.0.1"), address({127, 0, 0, 1}));

    ASSERT_THROW(address("444.4422.22.223"), std::invalid_argument);
    ASSERT_THROW(address("256.0.0.0"), std::invalid_argument);
    ASSERT_THROW(address("0.0.0.0 trailing_characters"), std::invalid_argument);
    ASSERT_THROW(address("0..0.0"), std::invalid_argument);
    ASSERT_THROW(address("leading_characters0.0.0.0"), std::invalid_argument);
    ASSERT_THROW(address("000.00.11.11"), std::invalid_argument);
}

TEST(IPv4Address, ToString) {
    using namespace libwire;

    // Basic test.
    // Also: We should "normalize" output, so only one zero is allowed.
    ASSERT_EQ(address({127, 0, 0, 1}).to_string(), "127.0.0.1");
}

TEST(IPv6Address, VersionInitialization) {
    using namespace libwire;

    address adr1({0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1});
    ASSERT_EQ(adr1.version, ip::v6);
}

TEST(IPv6Address, FromString) {
    using namespace libwire;

    address adr1({0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1});

    // Basic test
    ASSERT_EQ(address("::1"), adr1);

    // TODO: Check for other error conditions.

    // Incorrectly folded.
    ASSERT_THROW(address(":00::000:1"), std::invalid_argument);
}
