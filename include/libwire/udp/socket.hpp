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
#include <vector>
#include <libwire/error.hpp>
#include "libwire/internal/bsdsocket.hpp"

/*
 * If you had to open this file to find answer for your question - we are so
 * sorry. Please open issue with your question so we can update documentation
 * to answer it.
 */

/**
 * \file udp/socket.hpp
 *
 * This file defines udp::socket type, base class for outgoing UDP communication.
 */

namespace libwire::udp {
    /**
     * Descriptor wrapper for UDP socket.
     *
     * UDP (User Datagram Protocol) is a lightweight, unreliable,
     * datagram-oriented, connectionless protocol. It can be
     * used when reliability of TCP isn't important.
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
         * If socket allocationf ailed we will set socket to closed state.
         *
         * \note As opposed to TCP you should specify IP protocol version
         * here because we can't guess it from future connect() or bind() call
         * because there may be none. And we should not (re-)allocate socket in I/O
         * operations to prevent strange performance problems when you are jumping
         * between IPv6 and IPv4 too often.
         */
        socket(ip ipver) noexcept;

        socket(const socket&) = delete;
        socket(socket&&) noexcept = default;

        socket& operator=(const socket&) = delete;
        socket& operator=(socket&&) noexcept = default;

        /**
         * Deallocate socket.
         */
        ~socket();

        /**
         * Deallocate socket. This makes this instance unusable.
         */
        void close() noexcept;

        internal_::socket& implementation() noexcept;
        const internal_::socket& implementation() const noexcept;

        /**
         * Get native handle/descriptor for socket.
         *
         * Returned value is undefined if \ref is_open returns false.
         */
        internal_::socket::native_handle_t native_handle() const noexcept;

        /**
         * \name Socket options
         *
         * Several aspects of socket behavior can be changes by setting flags.
         *
         * See \ref tcp::socket documentation for detailed explanation of socket
         * options mechanism. There are no special options for UDP sockets.
         */
        ///@{

        /**
         * Query socket option value specified by type tag Option.
         *
         * Example:
         * \code
         * socket.option(tcp::no_delay); // => false by default.
         *
         * // Some options May have multiple values returned in tuple.
         * auto [ enabled, timeout ] = socket.option(tcp::linger);
         * \endcode
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
         * \code
         * socket.set_option(tcp::no_delay, true);
         *
         * // Some options may have multiple values:
         * socket.set_option(tcp::linger, true, 20s);
         * // ^ enable linger with 20 seconds timeout.
         * \endcode
         */
        template<typename Option, typename... Args>
        void set_option(const Option& /* tag */, Args&&... args) noexcept {
            Option::set(*this, std::forward<Args>(args)...);
        }
        ///@}

        /**
         * Associate UDP socket with remote endpoint.
         *
         * This will allow you to omit target endpoint from further
         * calls to write().
         * Also, socket will not receive datagrams sent from endpoints other
         * than specified.
         *
         * ec will be set to \ref error::invalid_argument if passed address
         * object have IP version different from one passed to socket constructor.
         */
        void associate(endpoint target, std::error_code& ec) noexcept;

        /**
         * Undo previous call to associate(). Have no effect if associate() is not used.
         */
        void disassociate() noexcept;

        /**
         * Accept datagrams coming on specified endpoint.
         *
         * ec will be set if something went wrong.
         */
        void listen(endpoint target, std::error_code& ec) noexcept;

        /**
         * \name Blocking I/O
         *
         * I/O functions in this category usually block thread until
         * operation is completed.
         */
        ///@{

        /**
         * Read pending datagram into buffer. If buffer is not large enough
         * to fit entire datagram it will be truncated. If buffer is smaller than
         * datagram it will be resized to datagram size.
         *
         * If src contains non-null pointer, datagram source endpoint will be
         * written to it.
         *
         * **Buffer type requirements:**
         *
         * Buffer must be container that encapsulates dynamic array,
         * so it must have data, size and resize member functions with
         * behavior as in std::vector.
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer& read(size_t bytes_count, Buffer&, std::error_code&, endpoint* source = nullptr) noexcept;

        /**
         * Same as overload with Buffer argument but return newly allocated
         * buffer every time.
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer read(size_t bytes_count, std::error_code&, endpoint* source = nullptr) noexcept;

        /**
         * Write contents of buffer to socket.
         *
         * Error code will be set if anything went wrong.
         *
         * dest argument will override destination specified using \ref associate.
         *
         * **Buffer type requirements**
         *
         * Buffer must be container that encapsulates dynamic array,
         * so it must have data and size member functions with
         * behavior as in std::vector.
         */
        template<typename Buffer = std::vector<uint8_t>>
        size_t write(const Buffer&, std::error_code&, const endpoint& dest = endpoint::invalid) noexcept;

#ifdef __cpp_exceptions
        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        void associate(endpoint target);

        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        void listen(endpoint target);

        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        template<typename Buffer = std::vector<uint8_t>>
        Buffer& read(size_t bytes_count, Buffer&, endpoint* src = nullptr);

        template<typename Buffer = std::vector<uint8_t>>
        Buffer read(size_t bytes_count, endpoint* src = nullptr);

        /**
         * Same as overload with error code but throws std::system_error
         * instead of setting error code argument.
         */
        template<typename Buffer = std::vector<uint8_t>>
        size_t write(const Buffer&, const endpoint& dest = endpoint::invalid);
#endif // ifdef __cpp_exceptions

        ///@}
    private:
        internal_::socket impl;
    };

    template<typename Buffer>
    Buffer& socket::read(size_t bytes_count, Buffer& output, std::error_code& ec, endpoint* source) noexcept {
        static_assert(sizeof(std::remove_pointer_t<decltype(output.data())>) == sizeof(uint8_t),
                      "socket::read can't be used with container with non-byte elements");

        output.resize(bytes_count);
        size_t bytes_received;
        if (source == nullptr) {
            bytes_received = impl.read(output.data(), bytes_count, ec);
        } else {
            bytes_received = impl.recvfrom(output.data(), bytes_count, *source, ec);
        }
        if (ec) return output;
        output.resize(bytes_received);

        return output;
    }

    template<typename Buffer>
    Buffer socket::read(size_t bytes_count, std::error_code& ec, endpoint* source) noexcept {
        Buffer buffer{};
        return read(bytes_count, buffer, ec, source);
    }

    extern template std::vector<uint8_t> socket::read(size_t, std::error_code&, endpoint* source);
    extern template std::string socket::read(size_t, std::error_code&, endpoint* source);

    extern template std::vector<uint8_t>& socket::read(size_t, std::vector<uint8_t>&, std::error_code&,
                                                       endpoint* source);
    extern template std::string& socket::read(size_t, std::string&, std::error_code&, endpoint*);

    template<typename Buffer>
    size_t socket::write(const Buffer& input, std::error_code& ec, const endpoint& dest) noexcept {
        static_assert(sizeof(std::remove_pointer_t<decltype(input.data())>) == sizeof(uint8_t),
                      "socket::write can't be used with container with non-byte elements");

        if (dest.is_invalid()) { // default value
            return impl.write(input.data(), input.size(), ec);
        } else {
            return impl.sendto(input.data(), input.size(), dest, ec);
        }
    }

    extern template size_t socket::write(const std::vector<uint8_t>&, std::error_code&, const endpoint&);
    extern template size_t socket::write(const std::string&, std::error_code&, const endpoint&);

#ifdef __cpp_exceptions
    template<typename Buffer>
    Buffer& socket::read(size_t bytes_count, Buffer& output, endpoint* source) {
        std::error_code ec;
        auto res = read<Buffer>(bytes_count, output, ec, source);
        if (ec) throw std::system_error(ec);
        return output;
    }

    template<typename Buffer>
    Buffer socket::read(size_t bytes_count, endpoint* source) {
        Buffer buffer{};
        return read(bytes_count, buffer, source);
    }

    extern template std::vector<uint8_t>& socket::read(size_t, std::vector<uint8_t>&, endpoint* source);
    extern template std::string& socket::read(size_t, std::string&, endpoint* source);

    extern template std::vector<uint8_t> socket::read(size_t, endpoint* source);
    extern template std::string socket::read(size_t, endpoint* source);

    template<typename Buffer>
    size_t socket::write(const Buffer& input, const endpoint& dest) {
        std::error_code ec;
        size_t res = write<Buffer>(input, ec, dest);
        if (ec) throw std::system_error(ec);
        return res;
    }

    extern template size_t socket::write(const std::vector<uint8_t>&, const endpoint&);
    extern template size_t socket::write(const std::string&, const endpoint&);
#endif // ifdef __cpp_exceptions
} // namespace libwire::udp
