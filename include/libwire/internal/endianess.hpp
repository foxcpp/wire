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

/**
 * This file defines portable utilities for byte order conversion
 * to be used in libwire internals.
 *
 * Despite not being part of public API interface of these functions
 * probably will not be changed, so you can use them if you really
 * want.
 */

namespace libwire::internal_ {
    // Big endian to little endian and little endian to big endian.
    uint16_t flip_endianess(uint16_t) noexcept;
    uint32_t flip_endianess(uint32_t) noexcept;

    bool host_is_network() noexcept;

    template<typename T>
    inline T host_to_network(T input) noexcept {
        return host_is_network() ? input : flip_endianess(input);
    }

    template<typename T>
    inline T network_to_host(T input) noexcept {
        return host_is_network() ? input : flip_endianess(input);
    }
} // namespace libwire::internal_
