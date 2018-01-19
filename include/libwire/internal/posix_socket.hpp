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
#include <libwire/address.hpp>
#include <libwire/protocols.hpp>

namespace libwire::internal_ {
    /**
     * Thin C++ wrapper for POSIX sockets.
     *
     * Consult related POSIX Programmer's Manual pages for exact
     * behavior of each function.
     * * connect(3P) for socket::connect
     * * socket(3P) for socket::socket
     * * write(3P) for socket::write
     * * read(3P) for socket::read
     * * listen(3P) for socket::listen
     * etc...
     */
    struct socket {
        static unsigned max_pending_connections;

        /**
         * Construct handle without allocating socket.
         */
        socket() = default;

        explicit socket(int fd) : fd(fd) {}

        /**
         * Allocate new socket with specified family (network protocol)
         * and socket type (transport protocol).
         */
        socket(ip ipver, transport transport, std::error_code& ec);

        socket(const socket&) = delete;
        socket(socket&&) noexcept;
        socket& operator=(const socket&) = delete;
        socket& operator=(socket&&) noexcept;

        /**
         * Close underlying file descriptor if any.
         *
         * May block if there are unsent data left.
         */
        ~socket();

        /**
         * Connect socket to remote endpoint, set ec if any error
         * occured.
         */
        void connect(address target, uint16_t port, std::error_code& ec);

        /**
         * Shutdown read/write parts of full-duplex connection.
         */
        void shutdown(bool read = true, bool write = true);

        /**
         * Bind socket to local port using interface specified in interface_address,
         * set ec if any error occured.
         */
        void bind(uint16_t port, address interface_address, std::error_code& ec);

        /**
         * Start accepting connections on this listener socket.
         *
         * backlog is a hint for underlying implementation it can use
         * to limit number of pending connections in socket's queue.
         */
        void listen(int backlog, std::error_code& ec);

        /**
         * Extract and accept first connection from queue and create socket for it,
         * set ec if any error occured.
         */
        socket accept(std::error_code& ec);

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

        /**
         * Allows to check whether socket is intialized and can be operated on.
         */
        operator bool() const;

        static constexpr int not_initialized = -1;
        int fd = not_initialized;
    };
}
