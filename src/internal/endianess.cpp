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

#include "libwire/internal/endianess.hpp"

namespace libwire::internal_ {
    uint16_t flip_endianess(uint16_t input) noexcept {
        return (input >> 8u) | (input << 8u);
    }
    uint32_t flip_endianess(uint32_t input) noexcept {
        // clang-format off
        return (input >> 8u * 3u)                 |
               ((input << 8u * 1u) & 0x00FF0000u) |
               ((input >> 8u * 1u) & 0x0000FF00u) |
               (input << 8u * 3u);
        // clang-format on
    }

    bool host_is_network() noexcept {
        static const uint16_t probe = 1;
        // false if first byte is 1 (little endian).
        return *reinterpret_cast<const uint8_t*>(&probe) != 1;
    }
} // namespace libwire::internal_
