#include "libwire/internal/posix_socket.hpp"
#include <cassert>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include "libwire/internal/endianess.hpp"

namespace libwire::internal_ {
    socket::socket(net_protocol net_proto, transport transport, std::error_code& ec) {

        int domain;
        switch (net_proto) {
        case net_protocol::ipv4: domain = AF_INET; break;
        case net_protocol::ipv6: domain = AF_INET6; break;
        }

        int type, protocol;
        switch (transport) {
        case tcp: type = SOCK_STREAM; protocol = IPPROTO_TCP; break;
        case udp: type = SOCK_DGRAM; protocol = IPPROTO_UDP; break;
        }

        fd = ::socket(domain, type, protocol);
        if (fd < 0) {
            ec = std::error_code(errno, std::system_category());
        }
    }

    socket::~socket() {
        if (fd != not_initialized) close(fd);
    }

    void socket::connect(ipv4::address target, uint16_t port, std::error_code& ec) {
        static_assert(sizeof(ipv4::address) == sizeof(in_addr_t),
                      "Your platform uses some untrivial way to represent"
                      "IPv4 address, report this error.");

        assert(fd != not_initialized);

        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = host_to_network(port);
        address.sin_addr = *(in_addr*)(&target);

        int status = ::connect(fd, (sockaddr*)(&address), sizeof(address));
        if (status < 0) {
            ec = std::error_code(errno, std::system_category());
        }
    }

    void socket::bind(uint16_t port, ipv4::address interface_address, std::error_code& ec) {
        assert(fd != not_initialized);

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = host_to_network(port);
        address.sin_addr = *(in_addr*)(&interface_address);

        int status = ::bind(fd, (sockaddr*)(&address), sizeof(address));
        if (status < 0) {
            ec = std::error_code(errno, std::system_category());
        }
    }

    void socket::listen(int backlog, std::error_code& ec) {
        assert(fd != not_initialized);

        int status = ::listen(fd, backlog);
        if (status < 0) {
            ec = std::error_code(errno, std::system_category());
        }
    }

    socket socket::accept(std::error_code& ec) {
        assert(fd != not_initialized);

        int accepted_fd = ::accept(fd, nullptr, nullptr);
        // TODO: Allow to get client address.

        if (accepted_fd < 0) {
            ec = std::error_code(errno, std::system_category());
            return socket();
        }

        return socket(accepted_fd);
    }

    size_t socket::write(const void* input, size_t length_bytes, std::error_code& ec) {
        assert(fd != not_initialized);

        ssize_t actually_written = ::write(fd, input, length_bytes);
        if (actually_written < 0) {
            ec = std::error_code(errno, std::system_category());
            return 0;
        }
        return actually_written;
    }

    size_t socket::read(void* output, size_t length_bytes, std::error_code& ec) {
        assert(fd != not_initialized);

        ssize_t actually_readen = ::read(fd, output, length_bytes);
        if (actually_readen < 0) {
            ec = std::error_code(errno, std::system_category());
            return 0;
        }
        return actually_readen;
    }
}
