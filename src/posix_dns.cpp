/*
 * Copyright © 2018 Maks Mazurov (fox.cpp) <foxcpp [at] yandex [dot] ru>
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
#include <cstring>
#include <cassert>
#include <netdb.h>

namespace libwire::dns {
    std::vector<address> resolve(ip protocol, const std::string_view& domain, std::error_code& ec) noexcept {
        addrinfo* result_raw = nullptr;

        int status = getaddrinfo(domain.data(), nullptr, nullptr, &result_raw);
        if (status != 0) {
            if (status == EAI_SYSTEM) {
                ec = std::error_code(errno, error::system_category());
            }
            ec = std::error_code(status, error::dns_category());
            assert(ec != error::unexpected);
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
