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

#include <tuple>
#include <libwire/error.hpp>
#include <libwire/address.hpp>
#include <libwire/internal/system_errors.hpp>
#include <libwire/internal/platform.hpp>

#if defined(LIBWIRE_POSIX)
#    include <sys/socket.h>
#    include <netinet/in.h>
#endif
#if defined(LIBWIRE_WINDOWS)
#    include <winsock2.h>
#    include <ws2tcpip.h>
#endif

/**
 * \file system_utils.hpp
 *
 * Set of misc utilities placed here because separate header for one function is too much.
 */

namespace libwire::internal_ {
    std::tuple<address, uint16_t> sockaddr_to_endpoint(sockaddr in);

    /**
     * Silently retry system call on EINTR, placing any other error in ec argument.
     */
    template<typename Func, typename... Args>
    auto error_wrapper(const Func& func, std::error_code& ec, Args&&... args) {
        ec = std::error_code();
        decltype(func(std::forward<Args>(args)...)) res;
        do {
            res = func(std::forward<Args>(args)...);
            ec = last_system_error();
        } while (ec == error::interrupted);
        return res;
    }
}

