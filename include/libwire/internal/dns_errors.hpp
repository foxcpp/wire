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
#include <string>
#include "libwire/error.hpp"

/**
 * Mapping of POSIX EAI_* codes to error conditions defined in error.hpp.
 */

namespace libwire::internal_ {
    /**
     * Get error code set by last called DNS resolver function.
     *
     * status is a value returned by DNS resolver function.
     *
     * Caller should not assume that value will be preserved after
     * call (this function *may* clear code).
     */
    std::error_code last_dns_error(int status) noexcept;

    class dns_errors : public std::error_category {
    public:
        const char* name() const noexcept override;
        std::string message(int code) const noexcept override;
        std::error_condition default_error_condition(int code) const noexcept override;
        bool equivalent(int code, const std::error_condition& condition) const noexcept override;
    };
} // namespace libwire::internal_
