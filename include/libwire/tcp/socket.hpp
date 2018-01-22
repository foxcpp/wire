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

#ifdef __unix__
    #include <libwire/internal/posix_socket.hpp>
#else
    #error "libwire doesn't supports socket interface on your platform. :("
#endif

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
        socket() noexcept = default;

        /**
         * Initialize socket from underlying raw handle.
         *
         * Used by tcp::listener for accept() function.
         * **Not part of the public API.**
         */
        socket(internal_::socket&& i) noexcept : implementation(std::move(i)) {}

        socket(const socket&) = delete;
        socket(socket&&) noexcept = default;

        socket& operator=(const socket&) = delete;
        socket& operator=(socket&&) noexcept = default;

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
        void connect(address, uint16_t port, std::error_code& ec) noexcept;

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
        void shutdown(bool read = true, bool write = true) noexcept;

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
        void close() noexcept;

        /**
         * Read up to bytes_count bytes from socket into buffer passed by
         * reference. Buffer will be resized to actual count of bytes
         * received.
         *
         * Error code will be set to error code if anything went wrong, buffer
         * will be resized to 0 elements.
         *
         * **Buffer type requirements:**
         *
         * Buffer must be container that encapsulates dynamic array,
         * so it must have data, size and resize member functions with
         * behavior as in std::vector.
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer& read(size_t bytes_count, Buffer&, std::error_code&) noexcept;

        /**
         * Same as overload with Buffer argument but return newly allocated
         * buffer every time.
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer read(size_t bytes_count, std::error_code&) noexcept;

        /**
         * Write contents of buffer to socket.
         *
         * Error code will be set if anything went wrong.
         *
         * Returns actual amount of bytes written, usually same as
         * buffer size unless socket is in non-blocking mode.
         *
         * **Buffer type requirements**
         *
         * Buffer must be container that encapsulates dynamic array,
         * so it must have data and size member functions with
         * behavior as in std::vector.
         */
        template<typename Buffer = std::vector<uint8_t>>
        size_t write(const Buffer&, std::error_code&) noexcept;

#ifdef __cpp_exceptions
        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        void connect(address, uint16_t port);

        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer& read(size_t bytes_count, Buffer&);

        template<typename Buffer = std::vector<uint8_t>>
        Buffer read(size_t bytes_count);

        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        template<typename Buffer = std::vector<uint8_t>>
        size_t write(const Buffer&);
#endif // ifdef __cpp_exceptions

    private:
        internal_::socket implementation;
    };

    template<typename Buffer>
    Buffer& socket::read(size_t bytes_count, Buffer& output, std::error_code& ec) noexcept {
        static_assert(sizeof(std::remove_pointer_t<decltype(output.data())>) == sizeof(uint8_t),
                      "socket::read can't be used with container with non-byte elements");

        // Resize buffer so it can store maximum amount of bytes we can receive.
        output.resize(bytes_count);

        size_t bytes_received = implementation.read(output.data(), bytes_count, ec);

        // Resize buffer to actual received size (implementtion returns 0 if error occured).
        output.resize(bytes_received);

        return output;
    }

    template<typename Buffer>
    Buffer socket::read(size_t bytes_count, std::error_code& ec) noexcept {
        Buffer buffer{};
        return read(bytes_count, buffer, ec);
    }

    extern template std::vector<uint8_t> socket::read(size_t, std::error_code&);
    extern template std::string socket::read(size_t, std::error_code&);

    extern template std::vector<uint8_t>& socket::read(size_t, std::vector<uint8_t>&, std::error_code&);
    extern template std::string& socket::read(size_t, std::string&, std::error_code&);

    template<typename Buffer>
    size_t socket::write(const Buffer& input, std::error_code& ec) noexcept {
        static_assert(sizeof(std::remove_pointer_t<decltype(input.data())>) == sizeof(uint8_t),
                      "socket::write can't be used with container with non-byte elements");

        return implementation.write(input.data(), input.size(), ec);
    }

    extern template size_t socket::write(const std::vector<uint8_t>&, std::error_code&);
    extern template size_t socket::write(const std::string&, std::error_code&);

#ifdef __cpp_exceptions
    template<typename Buffer>
    Buffer& socket::read(size_t bytes_count, Buffer& output) {
        std::error_code ec;
        auto res = read(bytes_count, output, ec);
        if (ec) throw std::system_error(ec);
        return output;
    }

    template<typename Buffer>
    Buffer socket::read(size_t bytes_count) {
        Buffer buffer{};
        return read(bytes_count, buffer);
    }

    extern template std::vector<uint8_t>& socket::read(size_t, std::vector<uint8_t>&);
    extern template std::string& socket::read(size_t, std::string&);

    extern template std::vector<uint8_t> socket::read(size_t);
    extern template std::string socket::read(size_t);

    template<typename Buffer>
    size_t socket::write(const Buffer& input) {
        std::error_code ec;
        size_t res = write(input, ec);
        if (ec) throw std::system_error(ec);
        return res;
    }

    extern template size_t socket::write(const std::vector<uint8_t>&);
    extern template size_t socket::write(const std::string&);
#endif // ifdef __cpp_exceptions
} // namespace libwire::tcp
