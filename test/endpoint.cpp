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
#include <libwire/endpoint.hpp>

using namespace libwire;

TEST(IPv4Endpoint, FromString) {
    // Basic test.
    endpoint ep("127.0.0.1:25565");
    ASSERT_EQ(ep.addr, address(127, 0, 0, 1));
    ASSERT_EQ(ep.port, 25565);

    ASSERT_THROW(endpoint("127.0.0.1:"), std::invalid_argument);
    ASSERT_THROW(endpoint("127.0.0.1:NOT_A_DIGIT"), std::invalid_argument);
    ASSERT_THROW(endpoint("127.0.0.1"), std::invalid_argument);
    ASSERT_THROW(endpoint(":25565"), std::invalid_argument);
    ASSERT_THROW(endpoint(":"), std::invalid_argument);
    ASSERT_THROW(endpoint(""), std::invalid_argument);
}

TEST(IPv6Endpoint, FromString) {
    endpoint ep("[::1]:25565");
    ASSERT_EQ(ep.addr, ipv6::loopback);
    ASSERT_EQ(ep.port, 25565);

    ASSERT_THROW(endpoint("[]:25565"), std::invalid_argument);
    ASSERT_THROW(endpoint("[:]:25565"), std::invalid_argument);
    ASSERT_THROW(endpoint("::1:25565"), std::invalid_argument);
    ASSERT_THROW(endpoint("[::]:"), std::invalid_argument);
    ASSERT_THROW(endpoint("[]:"), std::invalid_argument);
    ASSERT_THROW(endpoint("[]"), std::invalid_argument);
    ASSERT_THROW(endpoint(":"), std::invalid_argument);
    ASSERT_THROW(endpoint(""), std::invalid_argument);
}

TEST(IPv4Endpoint, ToString) {
    ASSERT_EQ(endpoint(ipv4::loopback, 25565).to_string(), "127.0.0.1:25565");
}

TEST(IPv6Endpoint, ToString) {
    ASSERT_EQ(endpoint(ipv6::loopback, 25565).to_string(), "[::1]:25565");
}