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

#include "libwire/tcp/options.hpp"
#include "libwire/tcp/socket.hpp"
#include <cassert>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

/**
 * This file implements options specified in options.hpp header
 * for POSIX-ish platforms such as Linux and BSD.
 */

// TODO (PR's welcomed): There is a lot of code duplication, find a way to clean
// it.

using namespace std::literals::chrono_literals;

namespace libwire::tcp {
    inline namespace options {
        void keep_alive_t::set(socket& sock, bool enabled) noexcept {
            setsockopt(sock.native_handle(), IPPROTO_TCP, TCP_NODELAY, &enabled, sizeof(bool));
        }

        bool keep_alive_t::get(const socket& sock) noexcept {
            int result;
            socklen_t result_size = sizeof(result);
            getsockopt(sock.native_handle(), SOL_SOCKET, SO_KEEPALIVE, &result, &result_size);
            assert(result_size == sizeof(result));
            return bool(result);
        }

        void linger_t::set_impl(socket& sock, bool enabled, std::chrono::seconds timeout) noexcept {
            struct linger linger_opt {};
            linger_opt.l_onoff = int(enabled);
            linger_opt.l_linger = int(timeout.count());
            setsockopt(sock.native_handle(), SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));
        }

        std::tuple<bool, std::chrono::seconds> linger_t::get(const socket& sock) noexcept {
            struct linger linger_opt {};
            socklen_t opt_size = sizeof(linger_opt);
            getsockopt(sock.native_handle(), SOL_SOCKET, SO_LINGER, &linger_opt, &opt_size);
            assert(opt_size == sizeof(linger_opt));
            return {bool(linger_opt.l_onoff), std::chrono::seconds(linger_opt.l_linger)};
        }

        void timeout_t::set_impl(socket& sock, std::chrono::milliseconds timeout) noexcept {
#ifdef TCP_USER_TIMEOUT
            auto timeout_count = unsigned(timeout.count());
            setsockopt(sock.native_handle(), IPPROTO_TCP, TCP_USER_TIMEOUT, &timeout_count, sizeof(timeout_count));
#else
            (void)sock;
            (void)timeout; // Silence "unused parameter" warnings.
#endif
        }

        std::chrono::milliseconds timeout_t::get(const socket& sock) noexcept {
#ifdef TCP_USER_TIMEOUT
            unsigned result;
            socklen_t result_size = sizeof(result);
            getsockopt(sock.native_handle(), IPPROTO_TCP, TCP_USER_TIMEOUT, &result, &result_size);
            assert(result_size == sizeof(result));
            return std::chrono::milliseconds(result);
#else
            return std::chrono::duration_cast<std::chrono::milliseconds>(2h);
#endif
        }

        void no_delay_t::set(socket& sock, bool enabled) noexcept {
            setsockopt(sock.native_handle(), IPPROTO_TCP, TCP_NODELAY, &enabled, sizeof(bool));
        }

        bool no_delay_t::get(const socket& sock) noexcept {
            int result;
            socklen_t result_size = sizeof(result);
            getsockopt(sock.native_handle(), IPPROTO_TCP, TCP_NODELAY, &result, &result_size);
            assert(result_size == sizeof(result));
            return bool(result);
        }
    } // namespace options
} // namespace libwire::tcp
