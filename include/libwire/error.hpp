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

#include <system_error>

/**
 * \file error.hpp
 *
 * This header defines set of enumerations for platform-independent
 * error code handling.
 */

/*
 * If you had to open this file to find answer for your question - we are so
 * sorry. Please open issue with your question so we can update documentation
 * to answer it.
 */

/**
 * Namespace with platform-independent error codes.
 *
 * \note Prefer to reference error conditions without enumeration name,
 * usually error names is self-explanatory.
 * I.e. error::success, not error::generic_condition::success.
 */
namespace libwire::error {
    /**
     * These values combine together different error codes to
     * simplify error handling where reaction to different
     * errors is same.
     */
    enum class generic : int8_t {
        // Implementation note:
        //  Negative enum values forced to ensure
        //  no collisions with other codes.

        /**
         * Failed to connect to destination server.
         *
         * Possible reasons:
         * * No server running on specified port (connection refused)
         * * IP address is invalid
         * * Host is known to be down
         * * Host is unreachable
         * * Timed out
         */
        no_destination = -1,

        /**
         * Out of resources.
         *
         * Possible reasons:
         * * Out of memory
         * * Limitation reached
         */
        no_resources = -2,

        /**
         * Disconnected, either forcibly or
         * gracefully.
         */
        disconnected = -3,
    };

    enum system_condition : uint8_t {
        /**
         * Operation completed successfully.
         */
        success = 0,

        /**
         * Error code not mapped to any other condition.
         *
         * \note std::error_code::message may still return
         * readable description.
         */
        unknown = 255,

        /**
         * Error code that should not occur on normal library
         * operation. Causes program termination in debug builds.
         *
         * Report it as a bug with std::error_code::message
         * output.
         */
        unexpected = 254,

        /**
         * Invalid argument passed to function.
         *
         * \note This error condition is mapped to multiple
         * platform error codes so you can use it to detect
         * any invalid argument.
         */
        invalid_argument = 1,

        /**
         * Access to **resource** disallowed.
         *
         * **Do not** confuse with \ref operation_not_permitted.
         *
         * Usually occurs when you try to bind to privileged (1-1024)
         * port on POSIX systems while running as non-privileged
         * user.
         *
         * Some sandboxing mechanisms can also restrict access to
         * networking, so it's better to ignore this error code
         * if access to resource is not required for program
         * operation.
         */
        permission_denied,

        /**
         * **Operation** disallowed.
         *
         * **Do not** confuse with \ref permission_denied.
         */
        operation_not_permitted,

        /**
         * Underlying implementation reported temporary failure -OR-
         * non-blocking operation would block.
         */
        try_again,

        /**
         * Non-blocking operation started, but not completed yet.
         */
        in_progress,

        /**
         * Previous non-blocking operation still running.
         */
        already,

        /**
         * System call interrupted by signal (POSIX-specific).
         */
        interrupted,

        /**
         * Operating system out of memory.
         */
        out_of_memory,

        /**
         * Per-process limitation reached.
         *
         * Can be emitted, for example, when you try to open
         * more sockets than allowed.
         */
        process_limit_reached,

        /**
         * System-wide limitation reached.
         *
         * Can be emitted, for example, when you try to open
         * more sockets than allowed.
         */
        system_limit_reached,

        /**
         * Requested protocol is not supported.
         *
         * Typically reported when system doesn't have
         * any IPv6-capable network interfaces.
         */
        protocol_not_supported,

        /**
         * Reported by \ref tcp::socket::connect to indicate that
         * no service is listening on specified port.
         */
        connection_refused,

        /**
         * RST segment received (TCP).
         */
        connection_reset,

        /**
         * Destination finished transmission.
         */
        broken_pipe,

        /**
         * Cannot send after transport endpoint shutdown.
         */
        shutdown,

        /**
         * Reported by listener::bind or listen to indicate
         * that port is already used.
         */
        already_in_use,

        /**
         * Reported by listener::bind to indicate that
         * interface with specified address doesn't
         * exists or not local.
         */
        address_not_available,

        /**
         * A connection has been aborted.
         */
        connection_aborted,

        /**
         * Network is unreachable.
         */
        network_unreachable,

        /**
         * Host is known to be down.
         */
        host_down,

        /**
         * Host destination is unreachable.
         */
        host_unreachable,

        /**
         * Timed out while trying to perform operation.
         */
        timeout,

        /**
         * Attempt to use not connected socket.
         */
        not_connected,

        /**
         * Remote side of connection finished transmission.
         */
        end_of_file,
    };

    enum dns_condition {
        /**
         * Requested host doesn't exists.
         */
        host_not_found,

        /**
         * Requested host doesn't exists, but DNS server answer is
         * non-authoritative.
         */
        host_not_found_try_again,

        /**
         * Requested host exists, but does not have any addresses
         * defined.
         */
        no_address,
    };

    std::error_condition make_error_condition(generic);
    std::error_condition make_error_condition(system_condition);
    std::error_condition make_error_condition(dns_condition);

    /**
     * Obtain reference to static instance of category for
     * DNS errors. name() will be "dns".
     */
    std::error_category& dns_category();

    /**
     * Obtain reference to static instance of category for
     * various network and socket errors. name() will be "system".
     */
    std::error_category& system_category();
} // namespace libwire::error

namespace std {
    template<>
    struct is_error_condition_enum<libwire::error::generic> : true_type {};

    template<>
    struct is_error_condition_enum<libwire::error::system_condition> : true_type {};

    template<>
    struct is_error_condition_enum<libwire::error::dns_condition> : true_type {};
} // namespace std
