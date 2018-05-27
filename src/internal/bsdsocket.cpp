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

#include "libwire/internal/bsdsocket.hpp"
#include <cassert>
#include "libwire/error.hpp"
#include "libwire/internal/platform.hpp"
#include "libwire/internal/system_utils.hpp"
#include "libwire/internal/system_errors.hpp"
#include "libwire/internal/endianess.hpp"

#if defined(LIBWIRE_POSIX)
#    include <unistd.h>
#    include <sys/socket.h>
#    include <netinet/ip.h>
#    define closesocket close
#endif
#if defined(LIBWIRE_WINDOWS)
#    include <winsock2.h>
#    include <ws2tcpip.h>
#    define SHUT_RD SD_RECEIVE
#    define SHUT_WR SD_SEND
#    define SHUT_RDWR SD_BOTH
#endif

namespace libwire::internal_ {
    unsigned socket::max_pending_connections = SOMAXCONN;

#if defined(LIBWIRE_WINDOWS)
    struct Initializer {
        Initializer() {
            WSAData wsa_data;
            int initres = WSAStartup(MAKEWORD(2, 2), &wsa_data);
            if (initres != 0) {
                ec = last_system_error();
            }
        }

        ~Initializer() {
            WSACleanup();
        }

        std::error_code ec;
    };
#endif

    socket::socket(ip ipver, transport transport, std::error_code& ec) noexcept {
#if defined(LIBWIRE_WINDOWS)
        static Initializer init;
        if (init.ec) ec = init.ec;
#endif

        int domain;
        switch (ipver) {
        case ip::v4: domain = AF_INET; break;
        case ip::v6: domain = AF_INET6; break;
        }

        int type, protocol;
        switch (transport) {
        case transport::tcp:
            type = SOCK_STREAM;
            protocol = IPPROTO_TCP;
            break;
        case transport::udp:
            type = SOCK_DGRAM;
            protocol = IPPROTO_UDP;
            break;
        }

        handle = error_wrapper(::socket, ec, domain, type, protocol);
        if (handle < 0) {
            return;
        }

#ifdef SO_NOSIGPIPE
        int one = 1;
        setsockopt(handle, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one));
#endif
    }

    socket::socket(socket&& o) noexcept {
        std::swap(o.handle, this->handle);
    }

    socket& socket::operator=(socket&& o) noexcept {
        std::swap(o.handle, this->handle);
        return *this;
    }

    socket::~socket() {
        if (handle != not_initialized) closesocket(handle);
    }

    socket::native_handle_t socket::native_handle() const noexcept {
        return handle;
    }

    void socket::shutdown(bool read, bool write) noexcept {
        assert(handle != not_initialized);

        int how = 0;
        if (read && !write) how = SHUT_RD;
        if (!read && write) how = SHUT_WR;
        if (read && write) how = SHUT_RDWR;
        assert(how != 0);

        int status = ::shutdown(handle, how);
        if (status < 0) {
            if (errno == EINTR) {
                shutdown(read, write);
                return;
            }
            // Other errors mean that something broken in library.
            assert(errno == ENOTCONN);
        }
    }

    void socket::connect(endpoint target, std::error_code& ec) noexcept {
        assert(handle != not_initialized);

        sockaddr_storage address = endpoint_to_sockaddr(target);

        error_wrapper(::connect, ec, handle, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    }

    void socket::bind(endpoint target, std::error_code& ec) noexcept {
        assert(handle != not_initialized);

        sockaddr_storage address = endpoint_to_sockaddr(target);

        error_wrapper(::bind, ec, handle, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    }

    void socket::listen(int backlog, std::error_code& ec) noexcept {
        assert(handle != not_initialized);

        error_wrapper(::listen, ec, handle, backlog);
    }

    socket socket::accept(std::error_code& ec) noexcept {
        assert(handle != not_initialized);

        native_handle_t accepted_fd = error_wrapper(::accept, ec, handle, nullptr, nullptr);
        // TODO (PR's welcomed): Allow to get client address.

        if (accepted_fd < 0) {
            return socket();
        }

        return socket(accepted_fd);
    }

#ifdef MSG_NOSIGNAL
#    define IO_FLAGS MSG_NOSIGNAL
#else
#    define IO_FLAGS 0
#endif

    size_t socket::write(const void* input, size_t length_bytes, std::error_code& ec) noexcept {
        assert(handle != not_initialized);

        ssize_t actually_written =
            error_wrapper(::send, ec, handle, reinterpret_cast<const char*>(input), length_bytes, IO_FLAGS);
        if (actually_written < 0) {
            return 0;
        }
        return size_t(actually_written);
    }

    size_t socket::read(void* output, size_t length_bytes, std::error_code& ec) noexcept {
        assert(handle != not_initialized);

        ssize_t actually_readen = ::recv(handle, reinterpret_cast<char*>(output), length_bytes, IO_FLAGS);
        // FIXME: Needs to be improved for non-blocking I/O.
        if (actually_readen == 0 && length_bytes != 0) {
            // We wanted more than zero bytes but got zero, looks like EOF.
            ec = std::error_code(EOF, error::system_category());
            return 0;
        }
        if (actually_readen < 0) {
            return 0;
        }
        return size_t(actually_readen);
    }

    size_t socket::sendto(const void* input, size_t length_bytes, endpoint dest, std::error_code& ec) noexcept {
        assert(handle != not_initialized);

        sockaddr_storage sockaddr_dest = endpoint_to_sockaddr(dest);

        ssize_t actually_written = error_wrapper(::sendto, ec, handle, (const char*)input, length_bytes, IO_FLAGS,
                                                 (sockaddr*)&sockaddr_dest, sizeof(sockaddr_dest));
        if (actually_written < 0) {
            return 0;
        }
        return size_t(actually_written);
    }

    size_t socket::recvfrom(void* output, size_t length_bytes, endpoint& source, std::error_code& ec) noexcept {
        assert(handle != not_initialized);

        sockaddr_storage sockaddr_src;
        socklen_t sockaddr_len = sizeof(sockaddr_src);

        ssize_t actually_readen = error_wrapper(::recvfrom, ec, handle, reinterpret_cast<char*>(output), length_bytes,
                                                IO_FLAGS, (sockaddr*)&sockaddr_src, &sockaddr_len);
        // FIXME: Needs to be improved for non-blocking I/O.
        if (actually_readen == 0 && length_bytes != 0) {
            // We wanted more than zero bytes but got zero, looks like EOF.
            ec = std::error_code(EOF, error::system_category());
            return 0;
        }
        if (actually_readen < 0) {
            return 0;
        }
        source = sockaddr_to_endpoint(sockaddr_src);
        return size_t(actually_readen);
    }

    socket::operator bool() const noexcept {
        return handle != not_initialized;
    }

    endpoint socket::local_endpoint() const noexcept {
        assert(handle != not_initialized);

        sockaddr_storage sock_address{};
        socklen_t length = sizeof(sock_address);
        [[maybe_unused]] int status = getsockname(handle, (sockaddr*)&sock_address, &length);
#ifndef NDEBUG // FIXME: This can explode with EINTR in production.
        if (status < 0) return {{0, 0, 0, 0}, 0u};
#endif
        return sockaddr_to_endpoint(sock_address);
    }

    endpoint socket::remote_endpoint() const noexcept {
        assert(handle != not_initialized);

        sockaddr_storage sock_address{};
        socklen_t length = sizeof(sock_address);
        [[maybe_unused]] int status = getpeername(handle, (sockaddr*)&sock_address, &length);
#ifndef NDEBUG // FIXME: This can explode with EINTR in production.
        if (status < 0) return {{0, 0, 0, 0}, 0u};
#endif
        return sockaddr_to_endpoint(sock_address);
    }

    void socket::disassociate() noexcept {
        assert(handle != not_initialized);

        sockaddr addr;
        addr.sa_family = AF_UNSPEC;
        ::connect(handle, &addr, sizeof(addr));
    }
} // namespace libwire::internal_
