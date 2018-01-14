/**
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

#include <cstdint>
#include <system_error>
#include <vector>
#include <libwire/internal/posix_socket.hpp>

namespace libwire::tcp {
    class socket {
    public:
        /**
         * Create new socket object.
         *
         * **Implementation Note:** Constructor itself doesn't
         * allocates socket descriptor. It's done on connect() call.
         * This is done to allow socket to be constructed without
         * knowing remote endpoint IP version ahead-of-time.
         */
        socket() = default;

        socket(const socket&) = delete;
        socket(socket&&) = default;

        socket& operator=(const socket&) = delete;
        socket& operator=(socket&&) = default;

        /**
         * Shutdown and then close socket.
         */
        ~socket();

        /**
         * Initialize underlying socket and connect to remote endpoint.
         *
         * \note If connect() called for already connected socket if
         * active connection will be closed by close() call.
         */
        void connect(ipv4::address, uint16_t port, std::error_code& ec);

        /**
         * Shutdown reading/writing part of full-duplex connection
         * (or both if read and wirte is true).
         *
         * This function have no effect if socket is not connected
         * ("Not connected" error is silently ignored).
         *
         * If both read and write arguments set to false behavior
         * is undefined.
         *
         * \note Socket can't be reused after shutdown, i.e.
         * you can't do connect() after shutdown().
         */
        void shutdown(bool read = true, bool write = true);

        /**
         * Close and destroy underlying socket.
         *
         * Closed socket object CAN be reused by calling connect().
         *
         * May block if socket has untransmitted data even if socket
         * is in non-blocking mode.
         *
         * \warning If you don't call shutdown() before close(), this
         * function will **forcibly** close the connection (RST will
         * be sent).
         */
        void close();

        /**
         * Reads up to bytes_count bytes from socket into buffer passed by
         * reference. Buffer will be resized to actual count of bytes
         * received.
         *
         * ec will be set to error code if anything went wrong, buffer
         * will be resized to 0 elements.
         *
         * **Buffer type requirements:**
         *
         * Buffer must be container that encapsulates dynamic array,
         * so it must have data() and size() member functions with
         * same behavior as std::vector::data() and std::vector::size()
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer& read(size_t bytes_count, Buffer&, std::error_code& ec);

        /**
         * Write contents of buffer to socket.
         *
         * **Buffer type requirements**
         *
         * Buffer must be container that encapsulates dynamic array,
         * so it must have data() and size() member functions with
         * same behavior as std::vector::data() and std::vector::size()
         */
        template<typename Buffer>
        size_t write(const Buffer&, std::error_code&);

    private:
        internal_::socket implementation;
    };

    template<typename Buffer>
    Buffer& socket::read(size_t bytes_count, Buffer& output, std::error_code& ec) {
        static_assert(sizeof(std::remove_pointer_t<decltype(output.data())>) == sizeof(uint8_t),
                      "socket::read can't be used with container with non-byte elements");

        // Resize buffer so it can store maximum amount of bytes we can receive.
        output.resize(bytes_count);

        size_t bytes_received = implementation.read(output.data(), bytes_count, ec);

        // Resize buffer to actual received size (implementtion returns 0 if error occured).
        output.resize(bytes_received);

        return output;
    }

    extern template std::vector<uint8_t>& socket::read(size_t, std::vector<uint8_t>&, std::error_code&);
    extern template std::string& socket::read(size_t, std::string&, std::error_code&);

    template<typename Buffer>
    size_t socket::write(const Buffer& input, std::error_code& ec) {
        static_assert(sizeof(std::remove_pointer_t<decltype(input.data())>) == sizeof(uint8_t),
                      "socket::write can't be used with container with non-byte elements");

        return implementation.write(input.data(), input.size(), ec);
    }

    extern template size_t socket::write(const std::vector<uint8_t>&, std::error_code&);
    extern template size_t socket::write(const std::string&, std::error_code&);
} // namespace libwire::tcp
