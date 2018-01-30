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

#include "libwire/error.hpp"

#if __has_include(<unistd.h>)
#    include "libwire/internal/error/posix_system_category.hpp"
#    include "libwire/internal/error/posix_dns_category.hpp"
#endif

namespace libwire::error {
    std::error_category& system_category() {
#if __has_include(<unistd.h>)
        static internal_::posix_system_category cat;
        return cat;
#endif
    }

    std::error_category& dns_category() {
#if __has_include(<unistd.h>)
        static internal_::posix_dns_category cat;
        return cat;
#endif
    }

    std::error_condition make_error_condition(generic val) {
        return std::error_condition(int(val), system_category());
    }

    std::error_condition make_error_condition(system_condition val) {
        return std::error_condition(int(val), system_category());
    }

    std::error_condition make_error_condition(dns_condition val) {
        return std::error_condition(int(val), dns_category());
    }
} // namespace libwire::error
