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

#pragma once

#include <cstdint>
#include <tuple>
#include <system_error>
#include <vector>
#include <libwire/error.hpp>

#ifdef __unix__
    #include <libwire/internal/posix_socket.hpp>
#else
    #error "libwire doesn't supports socket interface on your platform. :("
#endif

/*
 * If you had to open this file to find answer for your question - we are so
 * sorry. Please open issue with your question so we can update documentation
 * to answer it.
 */

/**
 * \file tcp/socket.hpp
 *
 * This file defines tcp::socket type, base class for outgoing TCP connections.
 */

namespace libwire::tcp {
    /**
     * Descriptor wrapper for TCP socket.
     *
     * TCP (Transmission Control Protocol) is a reliable, stream-oriented,
     * connection-oriented transport protocol. It is especially well
     * suited for continuous transmission of data.
     *
     * There is simplified graph of TCP socket states:
     * \image html libwire-tcp-flow.png
     *
     * ##### Thread-safety
     * * Distinct: safe
     * * Same: unsafe
     */
    class socket {
    public:
        /**
         * Create new socket object.
         *
         * **Implementation Note:** Constructor itself doesn't
         * allocates socket descriptor. It's done on \ref connect call.
         * This is done to allow socket to be constructed without
         * knowing remote endpoint IP version ahead-of-time.
         */
        socket() noexcept = default;

        /**
         * Initialize socket from underlying raw handle.
         *
         * Used by tcp::listener for \ref listener::accept function.
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
         * Get native handle/descriptor for socket.
         *
         * Returned value is undefined if \ref is_open returns false.
         */
        internal_::socket::native_handle_t native_handle() const noexcept;

        /**
         * Check whether underlying socket is open.
         *
         * \warning Even if \ref is_open returns true connection may be in
         * "half-open" state. Remote side possibly already called close()
         * and destroyed socket. In this case your next attempt to perform
         * I/O will result in broken_pipe/connection_reset/etc error (generic
         * for these errors is error::generic::disconnected).
         */
        bool is_open() const;

        /**
         * \name Socket options
         *
         * Several aspects of socket behavior can be changes by setting flags.
         *
         * For example: You can disable Nagle's algorithm and specify
         * smaller retransmission timeout using following code:
         * ```cpp
         * socket.set_option(tcp::no_delay, true);
         * socket.set_option(tcp::user_timeout, 500ms);
         * ```
         *
         * Even more, if some platform-specific option is not provided
         * by generic libwire interface you can add it easily because option
         * is just a class with two static functions.
         *
         * ```cpp
         * struct example_option_t {
         *      bool get(const tcp::socket& sock) const noexcept {
         *          return true;
         *      }
         *
         *      void set(tcp::socket& sock, bool value) const noexcept {
         *          // You can just not declare this function if option
         *          // can't be set.
         *
         *          // Use sock.native_handle() here to directly
         *          // interact with system API.
         *      }
         * } example_option{};
         * ```
         *
         * With example above you can write:
         * ```cpp
         * socket.set_option(example_option, true);
         * socket.option(example_option); // => true
         * ```
         */
        ///@{

        /**
         * Query socket option value specified by type tag Option.
         *
         * Example:
         * ```cpp
         * socket.option(tcp::no_delay); // => false by default.
         *
         * // Some options May have multiple values returned in tuple.
         * auto [ enabled, timeout ] = socket.option(tcp::linger);
         * ```
         */
        template<typename Option>
        auto option(const Option& /* tag */) const noexcept {
            return Option::get(*this);
        }

        /**
         * Set socket option value specified by type tag Option to
         * value value.
         *
         * Setting option not supported on current platform or specifying
         * invalid value results in undefined behavior. But usually new value
         * just ignored and corresponding option(tag) will return old value.
         *
         * Example:
         * ```cpp
         * socket.set_option(tcp::no_delay, true);
         *
         * // Some options may have multiple values:
         * socket.set_option(tcp::linger, true, 20s);
         * // ^ enable linger with 20 seconds timeout.
         * ```
         */
        template<typename Option, typename... Args>
        void set_option(const Option& /* tag */, Args&&... args) noexcept {
            Option::set(*this, {std::forward<Args>(args)...});
        }
        ///@}

        /**
         * \name Connection Endpoints Information
         *
         * Get address and port of local/remote end of connection.
         *
         * Pair of endpoints uniquely identifies **active** connection,
         * server can use remote_endpoint() to identify "sessions",
         * client can use local_endpoint() for same purpose.
         *
         * Return value is undefined if is_open() = false.
         *
         * **Example**
         * ```cpp
         * socket.connect({1, 2, 3, 4}, 5);
         * socket.remote_endpoint(); // Will return 1.2.3.4:5.
         * socket.local_endpoint(); // Will return LOCAL-IP:RANDOM-PORT.
         * ```
         */
        ///@{

        /**
         * Get address and port of local end of connection.
         *
         * Address is a local address of interface used for connection and
         * port is randomly picked from implementation-defined range of
         * [ephemeral ports](https://en.wikipedia.org/wiki/Ephemeral_port)
         * (usually 49152-65535).
         */
        std::tuple<address, uint16_t> local_endpoint() const noexcept;

        /**
         * Get address and port of remote end of connection.
         *
         * Usually same as address/port passed in \ref connect.
         */
        std::tuple<address, uint16_t> remote_endpoint() const noexcept;

        ///@}

        /**
         * Initialize underlying socket and connect to remote endpoint.
         *
         * \note If \ref connect called for already connected socket if
         * active connection will be closed.
         */
        void connect(address, uint16_t port, std::error_code& ec) noexcept;

        /**
         * Shutdown reading/writing part of full-duplex connection
         * (or both if read and write is true).
         *
         * This function have no effect if socket is not connected
         * ("Not connected" error is silently ignored).
         *
         * If both read and write arguments set to false behavior
         * is undefined.
         *
         * \note Socket can't be reused after shutdown, i.e.
         * you can't do \ref connect after \ref shutdown.
         */
        void shutdown(bool read = true, bool write = true) noexcept;

        /**
         * Close and destroy underlying socket.
         *
         * Closed socket object CAN be reused by calling \ref connect.
         *
         * May block if socket has untransmitted data even if socket
         * is in non-blocking mode.
         */
        void close() noexcept;

        /**
         * \name Blocking I/O
         *
         * I/O functions in this category usually block thread until
         * operation is completed.
         */
        ///@{

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
         * Read from socket until until gives byte is found or max_size
         * bytes read.
         *
         * \note Delimiter will be removed from socket stream but will
         * not be appended to buffer.
         *
         * \note max_size = 0 is a special case and means "no limit".
         *
         * **Buffer Type Requirements**
         *
         * size(), clear() and push_back() functions with behavior defined in
         * SequenceContainer concept.
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer& read_until(uint8_t delimiter, Buffer& buf, std::error_code&,
                           size_t max_size = 0) noexcept;

        /**
         * Same as overload with buffer argument but returns newly
         * allocated buffer every time.
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer read_until(uint8_t delimiter, std::error_code&,
                          size_t max_size = 0) noexcept;

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

        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer& read_until(uint8_t delimiter, Buffer& buf, size_t max_size = 0);

        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer read_until(uint8_t delimiter, size_t max_size = 0);
#endif // ifdef __cpp_exceptions

        ///@}
    private:
        internal_::socket implementation;

        // Used as internal socket state tracker.
        bool open = false;
    };

    template<typename Buffer>
    Buffer& socket::read(size_t bytes_count, Buffer& output, std::error_code& ec) noexcept {
        static_assert(sizeof(std::remove_pointer_t<decltype(output.data())>) == sizeof(uint8_t),
                      "socket::read can't be used with container with non-byte elements");

        // Resize buffer so it can store maximum amount of bytes we can receive.
        output.resize(bytes_count);

        size_t bytes_received = implementation.read(output.data(), bytes_count, ec);
        open = (ec != error::generic::disconnected);

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

        auto res = implementation.write(input.data(), input.size(), ec);
        open = (ec != error::generic::disconnected);
        return res;
    }

    extern template size_t socket::write(const std::vector<uint8_t>&, std::error_code&);
    extern template size_t socket::write(const std::string&, std::error_code&);

    template<typename Buffer>
    Buffer& socket::read_until(uint8_t delimiter, Buffer& buf, std::error_code& ec, size_t max_size) noexcept {
        uint8_t byte;
        memory_view view{&byte, 1};
        buf.clear();
        while (read(1, view, ec), !ec) {
            if (byte == delimiter) break;
            if (max_size != 0 && buf.size() == max_size) break;
            buf.push_back(byte);
        }
        return buf;
    }

    template<typename Buffer>
    Buffer socket::read_until(uint8_t delimiter, std::error_code& ec, size_t max_size) noexcept {
        Buffer buffer{};
        read_until(delimiter, buffer, ec, max_size);
        return buffer;
    }

    extern template std::vector<uint8_t> socket::read_until(uint8_t, std::error_code&, size_t);
    extern template std::string socket::read_until(uint8_t, std::error_code&, size_t);

    extern template std::vector<uint8_t>& socket::read_until(uint8_t, std::vector<uint8_t>&, std::error_code&, size_t);
    extern template std::string& socket::read_until(uint8_t, std::string&, std::error_code&, size_t);

#ifdef __cpp_exceptions
    template<typename Buffer>
    Buffer& socket::read(size_t bytes_count, Buffer& output) {
        std::error_code ec;
        auto res = read<Buffer>(bytes_count, output, ec);
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
        size_t res = write<Buffer>(input, ec);
        if (ec) throw std::system_error(ec);
        return res;
    }

    extern template size_t socket::write(const std::vector<uint8_t>&);
    extern template size_t socket::write(const std::string&);

    template<typename Buffer>
    Buffer& socket::read_until(uint8_t delimiter, Buffer& buf, size_t max_size) {
        std::error_code ec;
        read_until<Buffer>(delimiter, buf, ec, max_size);
        if (ec) throw std::system_error(ec);
        return buf;
    }

    template<typename Buffer>
    Buffer socket::read_until(uint8_t delimiter, size_t max_size) {
        std::error_code ec;
        auto res = read_until<Buffer>(delimiter, ec, max_size);
        if (ec) throw std::system_error(ec);
        return res;
    }

    extern template std::vector<uint8_t>& socket::read_until(uint8_t, std::vector<uint8_t>&, size_t);
    extern template std::string& socket::read_until(uint8_t, std::string&, size_t);

    extern template std::vector<uint8_t> socket::read_until(uint8_t, size_t);
    extern template std::string socket::read_until(uint8_t, size_t);
#endif // ifdef __cpp_exceptions
} // namespace libwire::tcp
