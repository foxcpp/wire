/*
 * Copyright © 2018 Max Mazurov (fox.cpp) <fox.cpp [at] disroot [dot] org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "gtest.hpp"
#include <libwire/address.hpp>

TEST(Ipv4Address, VersionInitialization) {
    using namespace libwire;

    address adr1({127, 0, 0, 1});
    ASSERT_EQ(adr1.version, ip::v4);
}

TEST(Ipv4Address, FromString) {
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

TEST(Ipv4Address, ToString) {
    using namespace libwire;

    // Basic test.
    // Also: We should "normalize" output, so only one zero is allowed.
    ASSERT_EQ(address({127, 0, 0, 1}).to_string(), "127.0.0.1");
}

TEST(Ipv6Address, VersionInitialization) {
    using namespace libwire;

    address adr1({0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1});
    ASSERT_EQ(adr1.version, ip::v6);
}

TEST(Ipv6Address, FromString) {
    using namespace libwire;

    address adr1({0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1});

    // Basic test
    ASSERT_EQ(address("::1"), adr1);

    // TODO: Check for other error conditions.

    // Incorrectly folded.
    ASSERT_THROW(address(":00::000:1"), std::invalid_argument);
}
