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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
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

#if __has_include(<unistd.h>)
    #include <unistd.h>
    #include <netdb.h>
    #include <cstring>
    #include <cassert>
#endif

namespace libwire::dns {
    std::vector<address> resolve(ip protocol, const std::string_view& domain, std::error_code& ec) {
#ifdef _POSIX_VERSION
        addrinfo hints;
        memset(&hints, 0x00, sizeof(hints));
        hints.ai_family = (protocol == ip::v4) ? AF_INET : AF_INET6;

        addrinfo* result_raw = nullptr;

        int status = getaddrinfo(domain.data(), nullptr, nullptr, &result_raw);
        if (status != 0) {
            ec = std::error_code(status, std::generic_category());
            return {};
        }

        std::vector<address> result;

        for (addrinfo* entry = result_raw; entry->ai_next != 0;
             entry = entry->ai_next) {

            assert(entry->ai_family == AF_INET || entry->ai_family == AF_INET6);

            if (entry->ai_family == AF_INET) {
                result.emplace_back(ip::v4, &((sockaddr_in*)entry->ai_addr)->sin_addr);
            } else if (entry->ai_family == AF_INET6) {
                result.emplace_back(ip::v6, &((sockaddr_in6*)entry->ai_addr)->sin6_addr);
            }
        }

        freeaddrinfo(result_raw);
        return result;
#else
    #error "libwire doesn't supports your platform. :("
#endif
    }
} // namespace libwire::dns
