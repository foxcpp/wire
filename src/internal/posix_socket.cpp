#include "libwire/internal/posix_socket.hpp"
#include <cassert>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include "libwire/internal/endianess.hpp"
#include "libwire/error.hpp"

namespace libwire::internal_ {
    static std::tuple<address, uint16_t> sockaddr_to_endpoint(sockaddr in) {
        if (in.sa_family == AF_INET) {
            auto sock_address_v4 = reinterpret_cast<sockaddr_in&>(in);
            return {memory_view(&sock_address_v4.sin_addr, sizeof(sock_address_v4.sin_addr)),
                    network_to_host(sock_address_v4.sin_port)};
        }
        if (in.sa_family == AF_INET6) {
            auto& sock_address_v6 = reinterpret_cast<sockaddr_in6&>(in);
            return {memory_view(&sock_address_v6.sin6_addr, sizeof(sock_address_v6.sin6_addr)),
                    network_to_host(sock_address_v6.sin6_port)};
        }
        assert(false);
    }

    unsigned socket::max_pending_connections = SOMAXCONN;

    socket::socket(ip ipver, transport transport, std::error_code& ec) noexcept {
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

        fd = ::socket(domain, type, protocol);
        if (fd < 0) {
            ec = std::error_code(errno, error::system_category());
            assert(ec != error::unexpected);
        }

#ifdef SO_NOSIGPIPE
        int one = 1;
        setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one));
#endif
    }

    socket::socket(socket&& o) noexcept {
        std::swap(o.fd, this->fd);
    }

    socket& socket::operator=(socket&& o) noexcept {
        std::swap(o.fd, this->fd);
        return *this;
    }

    socket::~socket() {
        if (fd != not_initialized) close(fd);
    }

    socket::native_handle_t socket::native_handle() const noexcept {
        return fd;
    }

    void socket::shutdown(bool read, bool write) noexcept {
        assert(fd != not_initialized);

        int how = 0;
        if (read && !write) how = SHUT_RD;
        if (!read && write) how = SHUT_WR;
        if (read && write) how = SHUT_RDWR;
        assert(how != 0);

        int status = ::shutdown(fd, how);
        if (status < 0) {
            if (errno == EINTR) {
                shutdown(read, write);
                return;
            }
            // Other errors mean that something broken in library.
            assert(errno == ENOTCONN);
        }
    }

    void socket::connect(address target, uint16_t port, std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        struct sockaddr_in address {};
        address.sin_family = AF_INET;
        address.sin_port = host_to_network(port);
        address.sin_addr = *reinterpret_cast<in_addr*>(target.parts.data());

        int status = ::connect(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
        if (status < 0) {
            if (errno == EINTR) {
                connect(target, port, ec);
                return;
            }
            ec = std::error_code(errno, error::system_category());
            assert(ec != error::unexpected);
        }
    }

    void socket::bind(uint16_t port, address interface_address, std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = host_to_network(port);
        address.sin_addr = *reinterpret_cast<in_addr*>(interface_address.parts.data());

        int status = ::bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
        if (status < 0) {
            if (errno == EINTR) {
                bind(port, interface_address, ec);
                return;
            }
            ec = std::error_code(errno, error::system_category());
            assert(ec != error::unexpected);
        }
    }

    void socket::listen(int backlog, std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        int status = ::listen(fd, backlog);
        if (status < 0) {
            if (errno == EINTR) {
                listen(backlog, ec);
                return;
            }
            ec = std::error_code(errno, error::system_category());
            assert(ec != error::unexpected);
        }
    }

    socket socket::accept(std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        int accepted_fd = ::accept(fd, nullptr, nullptr);
        // TODO (PR's welcomed): Allow to get client address.

        if (accepted_fd < 0) {
            if (errno == EINTR) {
                return accept(ec);
            }
            ec = std::error_code(errno, error::system_category());
            assert(ec != error::unexpected);
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
        assert(fd != not_initialized);

        ssize_t actually_written = ::send(fd, input, length_bytes, IO_FLAGS);
        if (actually_written < 0) {
            if (errno == EINTR) {
                return write(input, length_bytes, ec);
            }
            ec = std::error_code(errno, error::system_category());
            assert(ec != error::unexpected);
            return 0;
        }
        return size_t(actually_written);
    }

    size_t socket::read(void* output, size_t length_bytes, std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        ssize_t actually_readen = ::recv(fd, output, length_bytes, IO_FLAGS);
        if (actually_readen == 0 && length_bytes != 0) {
            // We wanted more than zero bytes but got zero, looks like EOF.
            ec = std::error_code(EOF, error::system_category());
            return 0;
        }
        if (actually_readen < 0) {
            if (errno == EINTR) {
                return read(output, length_bytes, ec);
            }
            ec = std::error_code(errno, error::system_category());
            assert(ec != error::unexpected);
            return 0;
        }
        return size_t(actually_readen);
    }

    socket::operator bool() const noexcept {
        return fd != not_initialized;
    }

    std::tuple<address, uint16_t> socket::local_endpoint() const noexcept {
        assert(fd != not_initialized);

        sockaddr sock_address{};
        socklen_t length = sizeof(sock_address);
        [[maybe_unused]] int status = getsockname(fd, &sock_address, &length);
#ifndef NDEBUG
        if (status < 0) return {{0, 0, 0, 0}, 0u};
#endif
        return sockaddr_to_endpoint(sock_address);
    }

    std::tuple<address, uint16_t> socket::remote_endpoint() const noexcept {
        assert(fd != not_initialized);

        sockaddr sock_address{};
        socklen_t length = sizeof(sock_address);
        [[maybe_unused]] int status = getpeername(fd, &sock_address, &length);
#ifndef NDEBUG
        if (status < 0) return {{0, 0, 0, 0}, 0u};
#endif
        return sockaddr_to_endpoint(sock_address);
    }
} // namespace libwire::internal_
