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

#include "libwire/udp/socket.hpp"

namespace libwire::udp {
    template std::vector<uint8_t>& socket::read(size_t, std::vector<uint8_t>&, std::error_code&, endpoint*);
    template std::string& socket::read(size_t, std::string&, std::error_code&, endpoint*);

    template std::vector<uint8_t> socket::read(size_t, std::error_code&, endpoint*);
    template std::string socket::read(size_t, std::error_code&, endpoint*);

    template size_t socket::write(const std::vector<uint8_t>&, std::error_code&, const endpoint&);
    template size_t socket::write(const std::string&, std::error_code&, const endpoint&);

    socket::socket(ip ipver) noexcept {
        std::error_code ec;
        impl = internal_::socket(ipver, transport::udp, ec);
        if (ec) {
            impl = internal_::socket();
        }
    }

    socket::~socket() {
        impl = internal_::socket();
    }

    internal_::socket::native_handle_t socket::native_handle() const noexcept {
        return impl.native_handle();
    }

    void socket::associate(endpoint target, std::error_code& ec) noexcept {
        impl.connect(target, ec);
    }

    void socket::disassociate() noexcept {
        impl.disassociate();
    }

    void socket::listen(endpoint target, std::error_code& ec) noexcept {
        impl.bind(target, ec);
    }

    void socket::close() noexcept {
        // Reassignment to null socket will call destructor and
        // close destroyed socket.
        impl = internal_::socket();
    }

#ifdef __cpp_exceptions
    void socket::associate(endpoint target) {
        std::error_code ec;
        associate(target, ec);
        if (ec) throw std::system_error(ec);
    }

    void socket::listen(endpoint target) {
        std::error_code ec;
        listen(target, ec);
        if (ec) throw std::system_error(ec);
    }

    internal_::socket& socket::implementation() noexcept {
        return impl;
    }

    const internal_::socket& socket::implementation() const noexcept {
        return impl;
    }

    template std::vector<uint8_t>& socket::read(size_t, std::vector<uint8_t>&, endpoint*);
    template std::string& socket::read(size_t, std::string&, endpoint*);

    template std::vector<uint8_t> socket::read(size_t, endpoint*);
    template std::string socket::read(size_t, endpoint*);

    template size_t socket::write(const std::vector<uint8_t>&, const endpoint&);
    template size_t socket::write(const std::string&, const endpoint&);
#endif // ifdef __cpp_exceptions

} // namespace libwire::tcp
