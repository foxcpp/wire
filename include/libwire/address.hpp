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
#include <array>
#include <string_view>
#include <string>
#include <libwire/protocols.hpp>
#include <libwire/memory_view.hpp>

/**
 * \file address.hpp
 *
 * This file defines universal address structure for both IPv4 and IPv6
 * network addresses.
 */

/*
 * If you had to open this file to find answer for your question - we are so
 * sorry. Please open issue with your question so we can update documentation
 * to answer it.
 */

namespace libwire {
    /**
     * IPv4/IPv6 address structure.
     *
     * As you expect it can store both IPv6 and IPv4 addresses to make
     * library interface IP-version agnostic as much as possible.
     *
     * Address stored in as set of unsigned 8-bit numbers in parts array.
     * If IP version is IPv4 then only first 4 elements is used to store
     * 4 corresponding parts of address. For 127.0.0.1 parts array will
     * contain {127, 0, 0, 1, 0, ...}. Remaining bytes will be filled
     * with zeros. If IPv6 address used then all parts will be used as you
     * expect.
     *
     * If you have some hardcoded address then it's better to specify it
     * using initializer list e.g. {127, 0, 0, 1}. This will avoid runtime
     * overhead of string parsing.
     */
    struct address {
        /**
         * Uninitialized address value, is_invalid() = true.
         *
         * \note Any interaction with invalid address object other than \ref is_invalid and
         * comparsions have undefined behavior, but invalid object can be used as a
         * "null" value, since it will not be equal to any valid value.
         */
        address() = default;

        /**
         * Construct IP address from raw bytes in network byte order (big endian).
         * Size of view must be either 4 or 16 bytes.
         */
        address(const memory_view&) noexcept;

        /**
         * Construct IPv4 address from 4 bytes in network byte order (big endian).
         * Thus 127.0.0.1 => {127, 0, 0, 1}
         */
        address(uint8_t, uint8_t, uint8_t, uint8_t) noexcept;

        /**
         * Construct IPv6 address from 16 bytes in network byte order (big endian).
         * Thus ::1 => {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}
         *
         * \todo Sadly, there is no good way to short-cut it, if you can come up with
         * a solution without templates hell - PR's are welcome!
         */
        address(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t,
                uint8_t, uint8_t, uint8_t, uint8_t, uint8_t) noexcept;

        /**
         * Parse IP address from string. supports both IPv4 and IPv6,
         * use version member variable to determine address version.
         *
         * If exceptions are enabled - will throw std::invalid_argument,
         * otherwise constructed address will have is_invalid() = true.
         *
         * assume_ipver can be used to accept only one IP version (only
         * IPv4 addresses or only IPv6).
         *
         * \warning Multiple consecutive zeros are NOT allowed
         *  "000.0.11.11" will throw.
         */
        address(const std::string& text_ip, ip assume_ipver = ip(0)) noexcept(!LIBWIRE_EXCEPTIONS_ENABLED_BOOL);

        /**
         * Convert address object to string representation.
         *
         * For IPv6 it gives correctly compacted version.
         */
        std::string to_string() const noexcept;

        bool is_invalid() const noexcept;

        bool operator==(const address&) const noexcept;
        bool operator!=(const address&) const noexcept;

        ip version = ip(0);
        std::array<uint8_t, 16> parts{};

        static const address invalid;
    };

    /**
     * Namespace with few IPv4 address constants.
     */
    namespace ipv4 {
        inline address any = {0, 0, 0, 0}, broadcast = {255, 255, 255, 255}, loopback = {127, 0, 0, 1};
    } // namespace ipv4

    /**
     * Namespace with few IPv6 address constants.
     */
    namespace ipv6 {
        inline address any = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
                       broadcast = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
                       loopback = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1};
    } // namespace ipv6
} // namespace libwire

namespace std {
    /**
     * Hash implementation for address.
     */
    template<>
    struct hash<libwire::address> {
        std::size_t operator()(const libwire::address&) const noexcept;
    };
} // namespace std
