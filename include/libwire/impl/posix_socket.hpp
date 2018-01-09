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

#pragma once

#include <cstdint>
#include <system_error>
#include <libwire/ipv4/address.hpp>

namespace libwire::impl_ {
    /**
     * Thin C++ wrapper for POSIX sockets.
     *
     * Consult related POSIX Programmer's Manual pages for exact
     * behavior of each function.
     * * connect(3P) for posix_socket::connect
     * * socket(3P) for posix_socket::posix_socket
     * * write(3P) for posix_socket::write
     * * read(3P) for posix_socket::read
     */
    struct posix_socket {
        enum net_protocol {
            v4,
            v6
        };

        enum transport {
            tcp,
            udp,
        };

        /**
         * Construct handle without allocating socket.
         */
        posix_socket() = default;

        /**
         * Allocate new socket with specified family (network protocol)
         * and socket type (transport protocol).
         */
        posix_socket(net_protocol ipver, transport transport, std::error_code& ec);

        posix_socket(const posix_socket&) = delete;
        posix_socket(posix_socket&&) = default;
        posix_socket& operator=(const posix_socket&) = delete;
        posix_socket& operator=(posix_socket&&) = default;

        /**
         * Close underlying file descriptor if any.
         *
         * May block if there are unsent data left.
         */
        ~posix_socket();

        /**
         * Connect socket to remote endpoint, set ec if any error
         * occured.
         */
        void connect(ipv4::address target, uint16_t port, std::error_code& ec);

        /**
         * Write length_bytes from input to socket, set ec if any error
         * occured and return real count of data written.
         */
        size_t write(const void* input, size_t length_bytes, std::error_code& ec);

        /**
         * Read up to length_bytes from input to socket, set ec if any error
         * occured and return real count of data readen.
         */
        size_t read(void* output, size_t length_bytes, std::error_code& ec);

        static constexpr int not_initialized = -1;
        int fd = not_initialized;
    };

}
