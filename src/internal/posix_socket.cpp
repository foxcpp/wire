#include "libwire/internal/posix_socket.hpp"
#include <cassert>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include "libwire/internal/endianess.hpp"
#include "libwire/error.hpp"

namespace libwire::internal_ {
    unsigned socket::max_pending_connections = SOMAXCONN;

    socket::socket(ip net_proto, transport transport, std::error_code& ec) noexcept {
        int domain;
        switch (net_proto) {
        case ip::v4: domain = AF_INET; break;
        case ip::v6: domain = AF_INET6; break;
        }

        int type, protocol;
        switch (transport) {
        case transport::tcp: type = SOCK_STREAM; protocol = IPPROTO_TCP; break;
        case transport::udp: type = SOCK_DGRAM; protocol = IPPROTO_UDP; break;
        }

        fd = ::socket(domain, type, protocol);
        if (fd < 0) {
            ec = std::error_code(errno, error::system_category());
            return;
        }

#if SO_NOSIGPIPE
        int one = 1;
        setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one));
#endif
    }

    socket::socket(socket&& o) noexcept {
        this->fd = o.fd;
        o.fd = not_initialized;
    }

    socket& socket::operator=(socket&& o) noexcept {
        this->fd = o.fd;
        o.fd = not_initialized;
        return *this;
    }

    socket::~socket() {
        if (fd != not_initialized) close(fd);
    }

    void socket::shutdown(bool read, bool write) noexcept {
        assert(fd != not_initialized);

        int how = 0;
        if (read && !write) how = SHUT_RD;
        if (!read && write) how = SHUT_WR;
        if (read && write)  how = SHUT_RDWR;
        assert(how != 0);

        int status = ::shutdown(fd, how);
        if (status < 0) {
            // Other errors mean that something broken in library.
            assert(errno ==  ENOTCONN);
        }
    }

    void socket::connect(address target, uint16_t port, std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = host_to_network(port);
        address.sin_addr = *reinterpret_cast<in_addr*>(target.parts.data());

        int status = ::connect(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
        if (status < 0) {
            ec = std::error_code(errno, error::system_category());
        }
    }

    void socket::bind(uint16_t port, address interface_address, std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        sockaddr_in address {};
        address.sin_family = AF_INET;
        address.sin_port = host_to_network(port);
        address.sin_addr = *reinterpret_cast<in_addr*>(interface_address.parts.data());

        int status = ::bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
        if (status < 0) {
            ec = std::error_code(errno, error::system_category());
        }
    }

    void socket::listen(int backlog, std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        int status = ::listen(fd, backlog);
        if (status < 0) {
            ec = std::error_code(errno, error::system_category());
        }
    }

    socket socket::accept(std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        int accepted_fd = ::accept(fd, nullptr, nullptr);
        // TODO: Allow to get client address.

        if (accepted_fd < 0) {
            ec = std::error_code(errno, error::system_category());
            return socket();
        }

        return socket(accepted_fd);
    }

#ifdef MSG_NOSIGNAL
    #define IO_FLAGS MSG_NOSIGNAL
#else
    #define IO_FLAGS 0
#endif

    size_t socket::write(const void* input, size_t length_bytes, std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        ssize_t actually_written = ::send(fd, input, length_bytes, IO_FLAGS);
        if (actually_written < 0) {
            ec = std::error_code(errno, error::system_category());
            return 0;
        }
        return actually_written;
    }

    size_t socket::read(void* output, size_t length_bytes, std::error_code& ec) noexcept {
        assert(fd != not_initialized);

        ssize_t actually_readen = ::recv(fd, output, length_bytes, IO_FLAGS);
        if (actually_readen < 0) {
            ec = std::error_code(errno, error::system_category());
            return 0;
        }
        return actually_readen;
    }

    socket::operator bool() const noexcept {
        return fd != not_initialized;
    }
} // namespace libwire::internal_
