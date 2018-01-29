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

#include <cstring>
#include "libwire/internal/error/posix_system_category.hpp"

const char* libwire::internal_::posix_system_category::name() const noexcept {
    return "system";
}

std::string libwire::internal_::posix_system_category::message(int code) const noexcept {
    if (code == EOF) {
        return "End of file";
    }

    return strerror(code);
}

std::error_condition libwire::internal_::posix_system_category::default_error_condition(int code) const noexcept {
#define MAP_CODE(errno_code, condition_code)\
    if (code == (errno_code)) return std::error_condition((condition_code), *this);

    // TODO: This table duplicated in two places. Find a way to share it without much pain.

    MAP_CODE(0,               error::success);
    MAP_CODE(EINVAL,          error::invalid_argument);
    MAP_CODE(EACCES,          error::permission_denied);
    MAP_CODE(EPERM,           error::operation_not_permitted);
    MAP_CODE(EAGAIN,          error::try_again);
    MAP_CODE(EWOULDBLOCK,     error::try_again);
    MAP_CODE(ENOBUFS,         error::out_of_memory);
    MAP_CODE(ENOMEM,          error::out_of_memory);
    MAP_CODE(EINPROGRESS,     error::in_progress);
    MAP_CODE(EALREADY,        error::already);
    MAP_CODE(EINTR,           error::interrupted);
    MAP_CODE(EMFILE,          error::process_limit_reached);
    MAP_CODE(ENFILE,          error::system_limit_reached);
    MAP_CODE(EPROTONOSUPPORT, error::protocol_not_supported);
    MAP_CODE(EAFNOSUPPORT,    error::protocol_not_supported);
    MAP_CODE(ECONNREFUSED,    error::connection_refused);
    MAP_CODE(EADDRINUSE,      error::already_in_use);
    MAP_CODE(EADDRNOTAVAIL,   error::address_not_available);
    MAP_CODE(ECONNABORTED,    error::connection_aborted);
    MAP_CODE(ECONNRESET,      error::connection_reset);
    MAP_CODE(EPIPE,           error::broken_pipe);
    MAP_CODE(ESHUTDOWN,       error::shutdown);
    MAP_CODE(EHOSTDOWN,       error::host_down);
    MAP_CODE(EHOSTUNREACH,    error::host_unreachable);

    MAP_CODE(EFAULT,       error::unexpected);
    MAP_CODE(EISCONN,      error::unexpected);
    MAP_CODE(EBADF,        error::unexpected);
    MAP_CODE(EPROTOTYPE,   error::unexpected);
    MAP_CODE(ENOTSOCK,     error::unexpected);
    MAP_CODE(EOPNOTSUPP,   error::unexpected);

    return std::error_condition(error::unknown, *this);
#undef MAP_CODE
}

bool libwire::internal_::posix_system_category::equivalent(int code, const std::error_condition& condition) const noexcept {
#define MAP_CODE(errno_code, condition_code)\
    if (code == (errno_code)) return condition.value() == (condition_code)
#define MAP_CODE_3(errno_code, condition_code, generic_code)\
    do {\
        if (code == (errno_code)) {\
            return condition.value() == (condition_code) ||\
                   condition.value() == int(generic_code);\
        }\
    } while (false)

    MAP_CODE  (0,               error::success);
    MAP_CODE  (EINVAL,          error::invalid_argument);
    MAP_CODE  (EACCES,          error::permission_denied);
    MAP_CODE  (EPERM,           error::operation_not_permitted);
    MAP_CODE  (EAGAIN,          error::try_again);
    MAP_CODE  (EWOULDBLOCK,     error::try_again);
    MAP_CODE_3(ENOBUFS,         error::out_of_memory, error::generic::no_resources);
    MAP_CODE_3(ENOMEM,          error::out_of_memory, error::generic::no_resources);
    MAP_CODE  (EINPROGRESS,     error::in_progress);
    MAP_CODE  (EALREADY,        error::already);
    MAP_CODE  (EINTR,           error::interrupted);
    MAP_CODE_3(EMFILE,          error::process_limit_reached, error::generic::no_resources);
    MAP_CODE_3(ENFILE,          error::system_limit_reached, error::generic::no_resources);
    MAP_CODE  (EPROTONOSUPPORT, error::protocol_not_supported);
    MAP_CODE  (EAFNOSUPPORT,    error::protocol_not_supported);
    MAP_CODE_3(ECONNREFUSED,    error::connection_refused, error::generic::no_destination);
    MAP_CODE  (EADDRINUSE,      error::already_in_use);
    MAP_CODE  (EADDRNOTAVAIL,   error::address_not_available);
    MAP_CODE_3(ECONNABORTED,    error::connection_aborted, error::generic::disconnected);
    MAP_CODE_3(ECONNRESET,      error::connection_reset, error::generic::disconnected);
    MAP_CODE_3(EPIPE,           error::broken_pipe, error::generic::disconnected);
    MAP_CODE_3(ESHUTDOWN,       error::shutdown, error::generic::disconnected);
    MAP_CODE_3(EHOSTDOWN,       error::host_down, error::generic::no_destination);
    MAP_CODE_3(EHOSTUNREACH,    error::host_unreachable, error::generic::no_destination);

    // Our custom code.
    MAP_CODE_3(EOF,             error::end_of_file, error::generic::disconnected);

    MAP_CODE(EFAULT,       error::unexpected);
    MAP_CODE(EISCONN,      error::unexpected);
    MAP_CODE(EBADF,        error::unexpected);
    MAP_CODE(EPROTOTYPE,   error::unexpected);
    MAP_CODE(ENOTSOCK,     error::unexpected);
    MAP_CODE(EOPNOTSUPP,   error::unexpected);

    return condition.value() == error::unknown;

#undef MAP_CODE
#undef MAP_CODE_3
}
