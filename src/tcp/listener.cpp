#include "libwire/tcp/listener.hpp"

namespace libwire::tcp {
    unsigned listener::max_backlog = internal_::socket::max_pending_connections;

    void listener::listen(address local_address, uint16_t port,
                          std::error_code& ec, unsigned max_backlog) {
        implementation = internal_::socket(local_address.version, transport::tcp, ec);
        if (ec) return;
        implementation.bind(port, local_address, ec);
        if (ec) return;
        implementation.listen(max_backlog, ec);
    }

    socket listener::accept(std::error_code& ec) {
        return {implementation.accept(ec)};
    }
} // namespace libwire::tcp
