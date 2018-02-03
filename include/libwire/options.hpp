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

#include <libwire/tcp/socket.hpp>

namespace libwire {
    /**
     * Dummy type for \ref nonblocking option.
     */
    struct non_blocking_t {
        template<typename Socket>
        static bool get(const Socket& sock) noexcept {
            return get_impl(sock.native_handle());
        }

        template<typename Socket>
        static void set(Socket& sock, bool value) noexcept {
            set_impl(sock.native_handle(), value);
        }

    private:
        static bool get_impl(internal_::socket::native_handle_t) noexcept;
        static void set_impl(internal_::socket::native_handle_t, bool) noexcept;
    };

    /**
     * Toggle non-blocking I/O mode on sockets.
     *
     * If this option enabled then read() and write() will
     * fail with error::would_block if operation can't be completed
     * immediately.
     *
     * If this option is disabled then read() can block if there is no
     * received data in system buffer. write() can block too if there is no
     * space left in buffer.
     */
    constexpr non_blocking_t non_blocking{};
} // namespace libwire
