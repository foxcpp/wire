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

#include <vector>
#include <system_error>
#include <string_view>
#include <libwire/protocols.hpp>
#include <libwire/address.hpp>

namespace libwire::dns {
    /**
     * Resolve domain name to one or more IP addresses of 'protocol' verison.
     *
     * \note Numeric IP addresses is accepted too and will be just copied
     * to output.
     *
     * \warning Currently error handling is broken, use ec only for success
     * check, no precise error code reported.
     */
    std::vector<address> resolve(ip protocol, const std::string_view& domain, std::error_code& ec);
} // namespace libwire::dns
