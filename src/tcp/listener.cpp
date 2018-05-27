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

#include "libwire/tcp/listener.hpp"

namespace libwire::tcp {
    void listener::listen(endpoint target, std::error_code& ec, unsigned max_backlog) noexcept {
        implementation = internal_::socket(target.address.version, transport::tcp, ec);
        if (ec) return;
        implementation.bind(target, ec);
        if (ec) return;
        implementation.listen(int(max_backlog), ec);
    }

    socket listener::accept(std::error_code& ec) noexcept {
        return {implementation.accept(ec)};
    }

    void listener::listen(endpoint target, unsigned max_backlog) {
        std::error_code ec;
        listen(target, ec, max_backlog);
        if (ec) throw std::system_error(ec);
    }

    socket listener::accept() {
        std::error_code ec;
        auto sock = accept(ec);
        if (ec) throw std::system_error(ec);
        return sock;
    }
} // namespace libwire::tcp
