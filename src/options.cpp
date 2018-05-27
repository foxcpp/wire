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

#include "libwire/options.hpp"
#include "libwire/internal/platform.hpp"

#if defined(LIBWIRE_POSIX)
#    include <fcntl.h>
#endif
#if defined(LIBWIRE_WINDOWS)
#    include <winsock2.h>
#    include <ws2tcpip.h>
#endif

namespace libwire {
    bool non_blocking_t::get_impl(internal_::socket& sock) noexcept {
#if defined(LIBWIRE_POSIX)
        int flags = fcntl(sock.native_handle(), F_GETFL, 0);
        return (flags & O_NONBLOCK) == O_NONBLOCK;
#endif
#if defined(LIBWIRE_WINDOWS)
        return sock.state.user_non_blocking;
#endif
    }

    void non_blocking_t::set_impl(internal_::socket& sock, bool enable) noexcept {
#if defined(LIBWIRE_POSIX)
        int flags = fcntl(sock.native_handle(), F_GETFL, 0);
        flags = enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
        fcntl(sock.native_handle(), F_SETFL, flags);
#endif
#if defined(LIBWIRE_WINDOWS)
        unsigned long mode = enable;
        ioctlsocket(sock.native_handle(), FIONBIO, &mode);
        sock.state.user_non_blocking = true;
#endif
    }
} // namespace libwire
