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

#include <libwire/internal/endianess.hpp>
#include <cassert>
#include "libwire/internal/system_utils.hpp"

std::tuple<libwire::address, uint16_t> libwire::internal_::sockaddr_to_endpoint(sockaddr in) {
    // FIXME: Broken for IPv6, sockaddr can't store IPv6 addresses but sockaddr_storage can.
    if (in.sa_family == AF_INET) {
        auto sock_address_v4 = reinterpret_cast<sockaddr_in&>(in);
        return {memory_view(&sock_address_v4.sin_addr, sizeof(sock_address_v4.sin_addr)),
                network_to_host(sock_address_v4.sin_port)};
    }
    if (in.sa_family == AF_INET6) {
        auto& sock_address_v6 = reinterpret_cast<sockaddr_in6&>(in);
        return {memory_view(&sock_address_v6.sin6_addr, sizeof(sock_address_v6.sin6_addr)),
                network_to_host(sock_address_v6.sin6_port)};
    }
    assert(false);
}

