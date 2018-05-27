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

#pragma once

#include <cstdint>
#include <tuple>
#include <system_error>
#include <libwire/address.hpp>
#include <libwire/protocols.hpp>
#include <libwire/internal/platform.hpp>
#include <libwire/endpoint.hpp>

namespace libwire::internal_ {
    /**
     * Thin C++ wrapper for BSD-like sockets with little hacks to work with Winsocks.
     */
    struct socket {
#if defined(LIBWIRE_POSIX)
        using native_handle_t = int;
#endif
#if defined(LIBWIRE_WINDOWS)
        using native_handle_t = unsigned long long;
        // Actually SOCKET type, but defined here as ULL to avoid inclusion of system headers.
#endif

        static unsigned max_pending_connections;

        /**
         * Constant native_handle() can be compared against to
         * check whether socket handle refers to alive socket.
         *
         * \note Prefer to use operator bool() for this check.
         */
#if defined(LIBWIRE_POSIX)
        static constexpr native_handle_t not_initialized = -1;
#endif
#if defined(LIBWIRE_WINDOWS)
        static constexpr native_handle_t not_initialized = ~0;
        // Actually INVALID_SOCKET, but defined here to avoid inclusion of system headers.
#endif

        /**
         * Construct handle without allocating socket.
         */
        socket() noexcept = default;

        explicit socket(native_handle_t fd) : handle(fd) {
        }

        /**
         * Allocate new socket with specified family (network protocol)
         * and socket type (transport protocol).
         */
        socket(ip ipver, transport transport, std::error_code& ec) noexcept;

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

        native_handle_t native_handle() const noexcept;

        /**
         * Connect socket to remote endpoint, set ec if any error
         * occurred.
         */
        void connect(endpoint target, std::error_code& ec) noexcept;

        /**
         * Shutdown read/write parts of full-duplex connection.
         */
        void shutdown(bool read = true, bool write = true) noexcept;

        /**
         * Bind socket to local port using interface specified in interface_address,
         * set ec if any error occurred.
         */
        void bind(endpoint target, std::error_code& ec) noexcept;

        /**
         * Start accepting connections on this listener socket.
         *
         * backlog is a hint for underlying implementation it can use
         * to limit number of pending connections in socket's queue.
         */
        void listen(int backlog, std::error_code& ec) noexcept;

        /**
         * Extract and accept first connection from queue and create socket for it,
         * set ec if any error occurred.
         */
        socket accept(std::error_code& ec) noexcept;

        /**
         * Write length_bytes from input to socket, set ec if any error
         * occurred and return real count of data written.
         */
        size_t write(const void* input, size_t length_bytes, std::error_code& ec) noexcept;

        /**
         * Read up to length_bytes from input to socket, set ec if any error
         * occurred and return real count of data read.
         */
        size_t read(void* output, size_t length_bytes, std::error_code& ec) noexcept;

        /**
         * Connect to AF_UNSPEC.
         * This will undo connect() for UDP socket.
         */
        void disassociate() noexcept;

        /**
         * Version of write for UDP sockets, uses dest instead of destination set using connect().
         */
        size_t sendto(const void* input, size_t length_bytes, endpoint dest, std::error_code& ec) noexcept;

        /**
         * Version of read for UDP sockets, writes datagram source to source tuple passed by reference.
         */
        size_t recvfrom(void* output, size_t length_bytes, endpoint& source, std::error_code& ec) noexcept;

        /**
         * Allows to check whether socket is initialized and can be operated on.
         */
        operator bool() const noexcept;

        endpoint local_endpoint() const noexcept;

        endpoint remote_endpoint() const noexcept;

        native_handle_t handle = not_initialized;

        struct state {
            // Set if user did set_option(non_blocking, ...);
            bool user_non_blocking : 1;
        } state{};
    };
} // namespace libwire::internal_
