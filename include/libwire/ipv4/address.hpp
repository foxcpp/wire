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
#include <string>
#include <string_view>

namespace libwire::ipv4 {
    struct address {
        address(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4);

        /**
         * Convert IPv4 address string to binary representation.
         *
         * Throws std::invalid_argument if ip_string is not a valid IPv4
         * address.
         *
         * \warning If libwire compiled with exceptions disabled parsing
         * errors will be ignored and result is undefined.
         */
        explicit address(const std::string_view& ip_string);

        /**
         * Convert IPv4 address string to binary representation.
         *
         * success set to true if ip_string parsed sucessfully,
         * or false otherwise.
         */
        address(const std::string_view& ip_string, bool& success);

        std::string to_string() const;

        bool operator==(const address&) const;
        bool operator!=(const address&) const;

        uint8_t parts[4] = { 0, 0, 0, 0 };
    };

    const inline address any  = { 0, 0, 0, 0 },
      broadcast  = { 255, 255, 255, 255 },
      loopback   = { 127, 0, 0, 1 };
} // namespace libwire::ipv4

