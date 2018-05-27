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

#include <unordered_set>
#include "gtest.hpp"
#include <libwire/dns.hpp>

TEST(DNSResolve, Error) {
    using namespace libwire;

    ASSERT_THROW(dns::resolve(ip::v4, "this-domain-will-never-exist"), std::system_error);
}

TEST(DNSResolve, NoMixedIPVersion) {
    using namespace libwire;

    std::unordered_set<address> unique_addresses;

    std::vector<address> result_v4 = dns::resolve(ip::v4, "example.com");
    for (const auto& address : result_v4) {
        ASSERT_EQ(address.version, ip::v4);
        unique_addresses.insert(address);
    }

    std::vector<address> result_v6 = dns::resolve(ip::v6, "example.com");
    for (const auto& address : result_v6) {
        ASSERT_EQ(address.version, ip::v6);
        unique_addresses.insert(address);
    }
}

TEST(DNSResolve, NoDuplicates) {
    using namespace libwire;

    std::unordered_set<address> unique_addresses;

    std::vector<address> result_v4 = dns::resolve(ip::v4, "example.com");
    for (const auto& address : result_v4) {
        unique_addresses.insert(address);
    }

    std::vector<address> result_v6 = dns::resolve(ip::v6, "example.com");
    for (const auto& address : result_v6) {
        unique_addresses.insert(address);
    }

    ASSERT_EQ(unique_addresses.size(), result_v4.size() + result_v6.size()) << "Duplicate IP addresses!\n";
}
