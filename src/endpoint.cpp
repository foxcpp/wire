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

#include "libwire/endpoint.hpp"
#include <cassert>
#include <cmath>

namespace libwire {
    const endpoint endpoint::invalid = {{0, 0, 0, 0}, 0};

    static uint16_t parse_port(const std::string_view& str) {
        uint32_t res = 0;
        for (auto i = 0; i != str.size(); ++i) {
            // We actually iterate it backwards.
            auto chr = str[str.size() - i - 1];
            if (!std::isdigit(chr)) {
                return 0;
            }
            res += (chr - '0') * std::pow(10, i);
        }
        if (res >= 65535) {
            return 0;
        }
        return uint16_t(res);
    }

    endpoint::endpoint(const struct address& addr, uint16_t port) : addr(addr), port(port) {
    }

    endpoint::endpoint(const std::string_view& str) noexcept(!LIBWIRE_EXCEPTIONS_ENABLED_BOOL) {
        // Smallest valid input: "[::]:1"
        if (str.size() < 6) {
            goto fail;
        }

        if (str[0] == '[') {
            // Look for ], parse everything in middle as a IPv6 address and after as a port.
            size_t bracket = str.find(']', 1);
            addr = libwire::address(std::string(str.substr(1, bracket - 1)), ip::v6);
            port = parse_port(str.substr(bracket + 2));
            if (port == 0) {
                goto fail;
            }
        } else if (std::isdigit(str[0])) {
            // Look for :, parse everything before as a IPv4 address and after as a port.
            size_t colon = str.find(':', 1);
            if (colon == std::string::npos) {
                goto fail;
            }
            addr = libwire::address(std::string(str.substr(0, colon)), ip::v4);
            port = parse_port(str.substr(colon + 1));
            if (port == 0) {
                goto fail;
            }
        } else {
            goto fail;
        }

        // Yes, we use goto because it's better than copy-pasting following code N times.
        return;
    fail:
        *this = invalid;
#ifdef __cpp_exceptions
        throw std::invalid_argument("invalid endpoint string");
#endif
    }

    bool endpoint::is_invalid() const noexcept {
        return addr.is_invalid() || port == 0;
    }

    std::string endpoint::to_string() const noexcept {
        assert(!is_invalid());
        // Code here may look strange but this is because we try
        // hard to reduce count of result string reallocations.
        std::string res;
        if (addr.version == ip::v4) {
            res = addr.to_string();
            res.push_back(':');
            res += std::to_string(port);
        } else {
            res.reserve(45 + 2 + 5); // Max length of IPv6 address + [] and port.
            res.push_back('[');
            res += addr.to_string();
            res.push_back(']');
            res.push_back(':');
            res += std::to_string(port);
        }
        return res;
    }

    bool endpoint::operator==(const endpoint& rhs) const noexcept {
        return addr == rhs.addr && port == rhs.port;
    }

    bool endpoint::operator!=(const endpoint& rhs) const noexcept {
        return addr != rhs.addr || port != rhs.port;
    }
} // namespace libwire
