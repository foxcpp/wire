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

#include "libwire/tcp/socket.hpp"

namespace libwire::tcp {
    template std::vector<uint8_t>& socket::read(size_t, std::vector<uint8_t>&, std::error_code&);
    template std::string& socket::read(size_t, std::string&, std::error_code&);

    template std::vector<uint8_t> socket::read(size_t, std::error_code&);
    template std::string socket::read(size_t, std::error_code&);

    template size_t socket::write(const std::vector<uint8_t>&, std::error_code&);
    template size_t socket::write(const std::string&, std::error_code&);

    template std::vector<uint8_t> socket::read_until(uint8_t, std::error_code&, size_t);
    template std::string socket::read_until(uint8_t, std::error_code&, size_t);

    template std::vector<uint8_t>& socket::read_until(uint8_t, std::vector<uint8_t>&, std::error_code&, size_t);
    template std::string& socket::read_until(uint8_t, std::string&, std::error_code&, size_t);

    socket::socket(internal_::socket&& i) noexcept : implementation(std::move(i)) {
        open = (implementation.native_handle() != internal_::socket::not_initialized);
    }

    socket::~socket() {
        open = false;
        if (is_open()) shutdown();
        close();
    }

    internal_::socket::native_handle_t socket::native_handle() const noexcept {
        return implementation.native_handle();
    }

    bool socket::is_open() const {
        return this->open;
    }

    void socket::connect(endpoint target, std::error_code& ec) noexcept {
        implementation = internal_::socket(target.addr.version, transport::tcp, ec);
        if (ec) return;
        implementation.connect(target, ec);
        open = !ec;
    }

    void socket::close() noexcept {
        // Reassignment to null socket will call destructor and
        // close destroyed socket.
        implementation = internal_::socket();
        open = false;
    }

    void socket::shutdown(bool read, bool write) noexcept {
        implementation.shutdown(read, write);
    }

    endpoint socket::local_endpoint() const noexcept {
        return implementation.local_endpoint();
    }

    endpoint socket::remote_endpoint() const noexcept {
        return implementation.remote_endpoint();
    }

#ifdef __cpp_exceptions
    void socket::connect(endpoint target) {
        std::error_code ec;
        connect(target, ec);
        if (ec) throw std::system_error(ec);
    }

    template std::vector<uint8_t>& socket::read(size_t, std::vector<uint8_t>&);
    template std::string& socket::read(size_t, std::string&);

    template std::vector<uint8_t> socket::read(size_t);
    template std::string socket::read(size_t);

    template size_t socket::write(const std::vector<uint8_t>&);
    template size_t socket::write(const std::string&);

    template std::vector<uint8_t>& socket::read_until(uint8_t, std::vector<uint8_t>&, size_t);
    template std::string& socket::read_until(uint8_t, std::string&, size_t);

    template std::vector<uint8_t> socket::read_until(uint8_t, size_t);
    template std::string socket::read_until(uint8_t, size_t);
#endif // ifdef __cpp_exceptions

} // namespace libwire::tcp
