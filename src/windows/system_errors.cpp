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

#include "libwire/internal/system_errors.hpp"
#include <cstring>
#include <cassert>
#include <winsock2.h>
#include <ws2tcpip.h>

std::error_code libwire::internal_::last_system_error(int status) noexcept {
    std::error_code ec(WSAGetLastError(), libwire::error::system_category());
    assert(ec != error::unexpected);
    return ec;
}

const char* libwire::internal_::system_errors::name() const noexcept {
    return "system";
}

std::string libwire::internal_::system_errors::message(int code) const noexcept {
    if (code == EOF) {
        return "End of file";
    }

    return strerror(code);
}

std::error_condition libwire::internal_::system_errors::default_error_condition(int code) const noexcept {
#define MAP_CODE(errno_code, condition_code) \
    if (code == (errno_code)) return std::error_condition((condition_code), *this);

    // TODO: This table duplicated in two places. Find a way to share it without
    // much pain.

    // clang-format off
    MAP_CODE(0,                     error::success);
    MAP_CODE(WSAEINVAL,             error::invalid_argument);
    MAP_CODE(WSAEACCES,             error::permission_denied);
    MAP_CODE(WSAEWOULDBLOCK,        error::try_again);
    MAP_CODE(WSAENOBUFS,            error::out_of_memory);
    MAP_CODE(WSA_NOT_ENOUGH_MEMORY, error::out_of_memory);
    MAP_CODE(WSAEINPROGRESS,        error::in_progress);
    MAP_CODE(WSAEALREADY,           error::already);
    MAP_CODE(WSAEINTR,              error::interrupted);
    MAP_CODE(WSAEMFILE,             error::process_limit_reached);
    MAP_CODE(WSAEPROTONOSUPPORT,    error::protocol_not_supported);
    MAP_CODE(WSAEAFNOSUPPORT,       error::protocol_not_supported);
    MAP_CODE(WSAECONNREFUSED,       error::connection_refused);
    MAP_CODE(WSAEADDRINUSE,         error::already_in_use);
    MAP_CODE(WSAEADDRNOTAVAIL,      error::address_not_available);
    MAP_CODE(WSAECONNABORTED,       error::connection_aborted);
    MAP_CODE(WSAECONNRESET,         error::connection_reset);
    MAP_CODE(WSAESHUTDOWN,          error::shutdown);
    MAP_CODE(WSAEHOSTDOWN,          error::host_down);
    MAP_CODE(WSAEHOSTUNREACH,       error::host_unreachable);

    MAP_CODE(WSAEFAULT,       error::unexpected);
    MAP_CODE(WSAEISCONN,      error::unexpected);
    MAP_CODE(WSAEBADF,        error::unexpected);
    MAP_CODE(WSAEPROTOTYPE,   error::unexpected);
    MAP_CODE(WSAENOTSOCK,     error::unexpected);
    MAP_CODE(WSAEOPNOTSUPP,   error::unexpected);
    // clang-format on

    return std::error_condition(error::unknown, *this);
#undef MAP_CODE
}

bool libwire::internal_::system_errors::equivalent(int code, const std::error_condition& condition) const
    noexcept {
#define MAP_CODE(errno_code, condition_code) \
    if (code == (errno_code)) return condition.value() == (condition_code)
#define MAP_CODE_3(errno_code, condition_code, generic_code) \
    do { \
        if (code == (errno_code)) { \
            return condition.value() == (condition_code) || condition.value() == int(generic_code); \
        } \
    } while (false)

    // clang-format off
    MAP_CODE  (0,                      error::success);
    MAP_CODE  (WSAEINVAL,              error::invalid_argument);
    MAP_CODE  (WSAEACCES,              error::permission_denied);
    MAP_CODE  (WSAEWOULDBLOCK,         error::try_again);
    MAP_CODE_3(WSAENOBUFS,             error::out_of_memory, error::generic::no_resources);
    MAP_CODE_3(WSA_NOT_ENOUGH_MEMORY,  error::out_of_memory, error::generic::no_resources);
    MAP_CODE  (WSAEINPROGRESS,         error::in_progress);
    MAP_CODE  (WSAEALREADY,            error::already);
    MAP_CODE  (WSAEINTR,               error::interrupted);
    MAP_CODE_3(WSAEMFILE,              error::process_limit_reached, error::generic::no_resources);
    MAP_CODE  (WSAEPROTONOSUPPORT,     error::protocol_not_supported);
    MAP_CODE  (WSAEAFNOSUPPORT,        error::protocol_not_supported);
    MAP_CODE_3(WSAECONNREFUSED,        error::connection_refused, error::generic::no_destination);
    MAP_CODE  (WSAEADDRINUSE,          error::already_in_use);
    MAP_CODE  (WSAEADDRNOTAVAIL,       error::address_not_available);
    MAP_CODE_3(WSAECONNABORTED,        error::connection_aborted, error::generic::disconnected);
    MAP_CODE_3(WSAECONNRESET,          error::connection_reset, error::generic::disconnected);
    MAP_CODE_3(WSAESHUTDOWN,           error::shutdown, error::generic::disconnected);
    MAP_CODE_3(WSAEHOSTDOWN,           error::host_down, error::generic::no_destination);
    MAP_CODE_3(WSAEHOSTUNREACH,        error::host_unreachable, error::generic::no_destination);

    // Our custom code.
    MAP_CODE_3(EOF,    error::end_of_file, error::generic::disconnected);

    MAP_CODE(WSAEFAULT,       error::unexpected);
    MAP_CODE(WSAEISCONN,      error::unexpected);
    MAP_CODE(WSAEBADF,        error::unexpected);
    MAP_CODE(WSAEPROTOTYPE,   error::unexpected);
    MAP_CODE(WSAENOTSOCK,     error::unexpected);
    MAP_CODE(WSAEOPNOTSUPP,   error::unexpected);
    // clang-format on

    return condition.value() == error::unknown;

#undef MAP_CODE
#undef MAP_CODE_3
}
