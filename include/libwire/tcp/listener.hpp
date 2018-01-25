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

#include <libwire/tcp/socket.hpp>

/*
 * If you had to open this file to find answer for your question - we are so
 * sorry. Please open issue with your question so we can update documentation
 * to answer it.
 */

namespace libwire::tcp {
    /**
     * Restricted wrapper for TCP listening socket.
     *
     * "Restricted" because it exposes only API required for server.
     * You can start listening on port using listen() method and accept
     * connection from queue using accept() method.
     *
     * Quick usage example:
     * ```cpp
     * tcp::listener l{ipv4::any, 7777};
     * auto sock = l.accept();
     * ```
     *
     * #### Thread-safety
     * * Distinct: safe
     * * Same: unsafe
     */
    class listener {
    public:
        /**
         * Construct listener object.
         *
         * \note Due to some details socket is not allocated when listener() is called.
         */
        listener() noexcept = default;

        listener(const listener&) = delete;
        listener(listener&&) noexcept = default;

        listener& operator=(const listener&) = delete;
        listener& operator=(listener&&) noexcept = default;

        ~listener() = default;

        /**
         * Construct listener and start accepting connections.
         * See listen() documentation for arguments description.
         */
        inline listener(address local_endpoint, uint16_t port,
                        std::error_code& ec,
                        unsigned backlog = internal_::socket::max_pending_connections) noexcept {

            listen(local_endpoint, port, ec, backlog);
        }

        inline listener(address local_endpoint, uint16_t port,
                        unsigned backlog = internal_::socket::max_pending_connections) {

            listen(local_endpoint, port, backlog);
        }

        /**
         * Start listening for incoming connections on specified
         * endpoint. backlog argument sets maximum size of
         * pending connections queue.
         *
         * Any errors occurred (invalid address or already used port, for example)
         * will be reported through ec argument.
         *
         * #### Implementation Note
         *
         * This function combines bind() and listen() of underlying
         * socket API.
         */
        void listen(address local_address, uint16_t port,
                    std::error_code& ec,
                    unsigned backlog = internal_::socket::max_pending_connections) noexcept;

        /**
         * Accept first connection from listener queue and create
         * socket for it.
         *
         * Any errors occurred (open sockets limit hit, for example)
         * will be reported through ec argument.
         */
        socket accept(std::error_code& ec) noexcept;

#ifdef __cpp_exceptions
        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        socket accept();

        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        void listen(address local_address, uint16_t port,
                    unsigned backlog = internal_::socket::max_pending_connections);
#endif // ifdef __cpp_exceptions

    private:
        internal_::socket implementation;
    };
} // namespace libwire::tcp

