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
#include "libwire/endpoint.hpp"
#include "libwire/internal/system_utils.hpp"

namespace libwire::internal_ {
    endpoint sockaddr_to_endpoint(sockaddr_storage in) {
        if (in.ss_family == AF_INET) {
            auto sock_address_v4 = reinterpret_cast<sockaddr_in&>(in);
            return {memory_view(&sock_address_v4.sin_addr, sizeof(sock_address_v4.sin_addr)),
                    network_to_host(sock_address_v4.sin_port)};
        }
        if (in.ss_family == AF_INET6) {
            auto& sock_address_v6 = reinterpret_cast<sockaddr_in6&>(in);
            return {memory_view(&sock_address_v6.sin6_addr, sizeof(sock_address_v6.sin6_addr)),
                    network_to_host(sock_address_v6.sin6_port)};
        }
        assert(false);
    }

    sockaddr_storage endpoint_to_sockaddr(const endpoint& in) {
        sockaddr_storage res;
        res.ss_family = AF_UNSPEC;
        if (in.addr.version == ip::v4) {
            res.ss_family = AF_INET;
            ((sockaddr_in*)(&res))->sin_addr = *((in_addr*)in.addr.parts.data());
            ((sockaddr_in*)(&res))->sin_port = host_to_network(in.port);
        }
        if (in.addr.version == ip::v6) {
            res.ss_family = AF_INET6;
            ((sockaddr_in6*)(&res))->sin6_addr = *((in6_addr*)in.addr.parts.data());
            ((sockaddr_in6*)(&res))->sin6_port = host_to_network(in.port);
        }
        return res;
    }
} // namespace libwire::internal_