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

#include "libwire/dns.hpp"
#include "libwire/error.hpp"
#include "libwire/internal/platform.hpp"
#include "libwire/internal/dns_errors.hpp"
#include <cstring>
#include <cassert>

#if defined(LIBWIRE_POSIX)
#   include <netdb.h>
#endif
#if defined(LIBWIRE_WINDOWS)
#   include <winsock2.h>
#   include <ws2tcpip.h>

#endif

namespace libwire::dns {
    std::vector<address> resolve(ip protocol, const std::string_view& domain, std::error_code& ec) noexcept {
        addrinfo* result_raw = nullptr;

        int status = getaddrinfo(domain.data(), nullptr, nullptr, &result_raw);
        if (status != 0) {
            ec = internal_::last_dns_error(status);
            return {};
        }

        std::vector<address> result;

        for (addrinfo* entry = result_raw; entry->ai_next != nullptr; entry = entry->ai_next) {
            assert(entry->ai_family == AF_INET || entry->ai_family == AF_INET6);

            if (entry->ai_socktype != SOCK_STREAM) continue;
            if (entry->ai_protocol != IPPROTO_TCP) continue;

            if (entry->ai_family == AF_INET && protocol == ip::v4) {
                result.emplace_back(memory_view(&(reinterpret_cast<sockaddr_in*>(entry->ai_addr))->sin_addr, 4));
            } else if (entry->ai_family == AF_INET6 && protocol == ip::v6) {
                result.emplace_back(memory_view(&(reinterpret_cast<sockaddr_in6*>(entry->ai_addr))->sin6_addr, 16));
            }
        }

        freeaddrinfo(result_raw);
        return result;
    }

    std::vector<address> resolve(ip protocol, const std::string_view& domain) {
        std::error_code ec;
        auto res = resolve(protocol, domain, ec);
        if (ec) throw std::system_error(ec);
        return res;
    }
} // namespace libwire::dns
