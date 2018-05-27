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

#include <libwire/memory_view.hpp>
#include <libwire/address.hpp>

namespace libwire {
    struct endpoint {
        address addr;
        uint16_t port;

        endpoint(const address& addr, uint16_t port);

        /**
         * Parse endpoint information from string representation.
         *
         * Input should have following format: "IP:PORT" (IPv4) or "[IP]:PORT" (IPv6).
         *
         * If exceptions are enabled - will throw std::invalid_argument,
         * otherwise constructed address will have is_invalid() = true.
         */
        endpoint(const std::string_view& str) noexcept(!LIBWIRE_EXCEPTIONS_ENABLED_BOOL);

        /**
         * Returns true if endpoint struct have invalid contents because
         * of c-tor failure.
         *
         * \note Invalid values can be safetly used as a "null" endpoint.
         */
        bool is_invalid() const noexcept;

        /**
         * Convert endpoint type to string representation.
         *
         * Result format is "IP:PORT" for IPv4 and "[IP]:PORT" for IPv6.
         */
        std::string to_string() const noexcept;

        bool operator==(const endpoint& rhs) const noexcept;
        bool operator!=(const endpoint& rhs) const noexcept;

        static const endpoint invalid;
    };
} // namespace libwire
