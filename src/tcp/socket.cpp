#include "libwire/tcp/socket.hpp"

namespace libwire::tcp {
    template std::vector<uint8_t>& socket::read(size_t, std::vector<uint8_t>&, std::error_code&);
    template std::string& socket::read(size_t, std::string&, std::error_code&);

    template size_t socket::write(const std::vector<uint8_t>&, std::error_code&);
    template size_t socket::write(const std::string&, std::error_code&);

    socket::~socket() {
        shutdown();
        close();
    }

    void socket::connect(ipv4::address target, uint16_t port, std::error_code& ec) {
        implementation = internal_::socket(net_protocol::ipv4, transport::tcp, ec);
        implementation.connect(target, port, ec);
    }

    void socket::close() {
        // Reassignment to null socket will call destructor and
        // close destroyed socket.
        implementation = internal_::socket();
    }

    void socket::shutdown(bool read, bool write) {
        implementation.shutdown(read, write);
    }
} // namespace libwire::tcp
