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
#include <array>
#include <string_view>
#include <libwire/protocols.hpp>

namespace libwire {
    struct address {
        /**
         * Construct IPv4 address from 4 bytes in network byte order (big endian).
         * Thus 127.0.0.1 => {127, 0, 0, 1}
         */
        address(uint8_t, uint8_t, uint8_t, uint8_t);

        /**
         * Construct IPv6 address from 16 bytes in network byte order (big endian).
         * Thus ::1 => {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1}
         *
         * \todo Sadly, there is no good way to short-cut it, if you can come up with
         * a solution without templates hell - PR's are welcome!
         */
        address(uint8_t, uint8_t, uint8_t, uint8_t,
                uint8_t, uint8_t, uint8_t, uint8_t,
                uint8_t, uint8_t, uint8_t, uint8_t,
                uint8_t, uint8_t, uint8_t, uint8_t);

        /**
         * Parse IP address from string. supports both IPv4 and IPv6,
         * use version member variable to determine address version.
         *
         * Throws std::invalid_argument if supplied string is not a
         * valid address.
         *
         * Defined as explicit because this is costly conversion.
         *
         * \warnings Multiple consecutive zeros is NOT allowed
         *  "000.0.11.11" will throw.
         */
        explicit address(const std::string_view& text_ip);

        /**
         * Parse IP address from string. supports both IPv4 and IPv6,
         * use version member variable to determine address version.
         *
         * Sets success to true if supplied address is valid, false
         * otherwise. Value of address is undefined if success is
         * false.
         *
         * \warnings Multiple consecutive zeros is NOT allowed
         *  "000.0.11.11" will set success to false.
         */
        address(const std::string_view& text_ip, bool& success);

        /**
         * Convert address object to string representation.
         *
         * For IPv6 it gives correctly compacted version.
         */
        std::string to_string() const;

        bool operator==(const address&) const;
        bool operator!=(const address&) const;

        net_protocol version;
        std::array<uint8_t, 16> parts;
    };

    namespace ipv4 {
        inline address any = {0, 0, 0, 0},
                       broadcast = {255, 255, 255, 255},
                       loopback = {127, 0, 0, 1};
    } // namespace ipv4

    namespace ipv6 {
        inline address any = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                              0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
                       broadcast = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
                       loopback = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1};
    } // namespace ipv6
} // namespace libwire
