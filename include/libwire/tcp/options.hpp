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

#include <chrono>
#include <tuple>

/*
 * If you had to open this file to find answer for your question - we are so
 * sorry. Please open issue with your question so we can update documentation
 * to answer it.
 */

/**
 * \file tcp/options.hpp
 *
 * This file defines set of options applicable for use with TCP sockets
 * using socket.set_option and socket.option.
 */

namespace libwire::tcp {
    class socket;

    /**
     * Inline namespace with options applicable with for TCP sockets.
     */
    inline namespace options {
        /**
         * Dummy type for \ref no_delay option.
         */
        struct no_delay_t {
            static void set(socket&, bool enabled) noexcept;

            static bool get(const socket&) noexcept;
        };

        /**
         * Disable the Nagle algorithm on TCP socket.
         *
         * This means that segments are always sent as soon as
         * possible, even if there is only a small amount of data. When not
         * set, data is buffered until there is a sufficient amount to send out,
         * thereby avoiding the frequent sending of small packets, which
         * results in poor utilization of the network.
         */
        constexpr no_delay_t no_delay{};

        /**
         * Dummy type for \ref user_timeout option.
         */
        struct timeout_t {
            void set(socket&, std::chrono::milliseconds) noexcept;

            std::chrono::milliseconds get(const socket&) noexcept;
        };

        /**
         * Set transmission timeout for TCP socket.
         *
         * This option takes an unsigned int as an argument. When the value is
         * greater than 0, it specifies the maximum amount of time in
         * milliseconds that transmitted data may remain unacknowledged before
         * TCP will forcibly close the corresponding connection and
         * return error::timeout to the application. If the option value is
         * specified as 0, TCP will to use the system default.
         *
         * \note Have no effect on systems which don't support this option.
         * Currently supported only on Linux 2.6+. socket.option(user_timeout)
         * will always return 0 on other systems.
         *
         * Usually you don't have to worry about timeouts, most systems by
         * default have giant transmission timeout (up to 2 hours on UNIXes).
         */
        constexpr timeout_t timeout{};

        /**
         * Dummy type for \ref keep_alive option.
         */
        struct keep_alive_t {
            static void set(socket&, bool) noexcept;

            static bool get(const socket&) noexcept;
        };

        /**
         * Enable keep-alive probes on TCP socket.
         *
         * After reaching some implementation-defined time of inactivity TCP
         * implementation will start to send keep-alive probes to make sure
         * other side is still up and can accept packets.
         */
        constexpr keep_alive_t keep_alive{};

        /**
         * Dummy type for linger option.
         */
        struct linger_t {
            static void set(socket&, bool enabled, std::chrono::seconds timeout) noexcept;

            static std::tuple<bool, std::chrono::seconds> get(const socket&) noexcept;
        };

        /**
         * Enable linger on socket.close on TCP socket.
         *
         * When enabled, a socket.close or socket.shutdown will not return until
         * all queued messages for the socket have been successfully sent or
         * the linger timeout has been reached. Otherwise, the call returns
         * immediately and the closing is done in the background.
         */
        constexpr linger_t linger{};
    } // namespace options
} // namespace libwire::tcp
