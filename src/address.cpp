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

#define _WIN32_WINNT 0x600 // Windows Vista+, required for inet_pton.

#include <libwire/address.hpp>
#include <cassert>
#include <algorithm>
#include "libwire/internal/platform.hpp"

#if defined(LIBWIRE_POSIX)
#    include <arpa/inet.h>
#endif
#if defined(LIBWIRE_WINDOWS)
#    include <winsock2.h>
#    include <ws2tcpip.h>
#endif

namespace libwire {
    const address address::invalid;

    address::address(const memory_view& mv) noexcept : parts{} {
        assert(mv.size() == 4 || mv.size() == 16);

        version = mv.size() == 4 ? ip::v4 : ip::v6;
        std::copy(mv.begin(), mv.end(), parts.begin());
    }

    address::address(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4) noexcept
        : version(ip::v4), parts{o1, o2, o3, o4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0} {
    }

    address::address(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4, uint8_t o5, uint8_t o6, uint8_t o7, uint8_t o8,
                     uint8_t o9, uint8_t o10, uint8_t o11, uint8_t o12, uint8_t o13, uint8_t o14, uint8_t o15,
                     uint8_t o16) noexcept
        : version(ip::v6), parts{o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16} {
    }

    address::address(const std::string& text_ip, ip assume_ipver) noexcept(!LIBWIRE_EXCEPTIONS_ENABLED_BOOL) {
        int family;
        if (assume_ipver == ip(0)) {
            family = AF_INET;
            version = ip::v4;
            for (const char& ch : text_ip) {
                if (ch == ':') {
                    family = AF_INET6;
                    version = ip::v6;
                }
            }
        } else {
            family = (assume_ipver == ip::v4) ? AF_INET : AF_INET6;
            version = assume_ipver;
        }

        if (inet_pton(family, text_ip.data(), parts.data()) <= 0) {
            *this = invalid;
#ifdef __cpp_exceptions
            throw std::invalid_argument("invalid address string");
#endif
        }
    }

    std::string address::to_string() const noexcept {
        assert(!is_invalid());
        int family = (version == ip::v4) ? AF_INET : AF_INET6;

        // 45 - maximum possible IPv6 address length.
        std::array<char, 45> buffer;

        // const_cast is required on Windows because function accepts PVOID (void*).
        const char* buf_ptr = inet_ntop(family, const_cast<uint8_t*>(parts.data()), buffer.data(), buffer.size());
        assert(buf_ptr != nullptr); // This function generally should not fail.

        return std::string(buffer.data());
    }

    bool address::operator==(const address& o) const noexcept {
        return version == o.version && parts == o.parts;
    }

    bool address::operator!=(const address& o) const noexcept {
        return version != o.version || parts != o.parts;
    }

    bool address::is_invalid() const noexcept {
        return version == ip(0);
    }
} // namespace libwire

namespace std { // NOLINT(cert-dcl58-cpp)
    std::size_t hash<libwire::address>::operator()(const libwire::address& addr) const noexcept {
        std::hash<uint8_t> hash;
        auto result = std::size_t(addr.version);
        for (const uint8_t& i : addr.parts) {
            // See
            // https://stackoverflow.com/questions/4948780/magic-number-in-boosthash-combine
            result ^= hash(i) + 0x9e3779b9u + (result << 6u) + (result >> 2u);
        }
        return result;
    }
} // namespace std
