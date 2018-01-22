#include "libwire/tcp/listener.hpp"

namespace libwire::tcp {
    void listener::listen(address local_address, uint16_t port,
                          std::error_code& ec, unsigned max_backlog) noexcept {
        implementation = internal_::socket(local_address.version, transport::tcp, ec);
        if (ec) return;
        implementation.bind(port, local_address, ec);
        if (ec) return;
        implementation.listen(int(max_backlog), ec);
    }

    socket listener::accept(std::error_code& ec) noexcept {
        return {implementation.accept(ec)};
    }

    void listener::listen(address local_address, uint16_t port,
                          unsigned max_backlog) {
        std::error_code ec;
        listen(local_address, port, ec, max_backlog);
        if (ec) throw std::system_error(ec);
    }

    socket listener::accept() {
        std::error_code ec;
        auto sock = accept(ec);
        if (ec) throw std::system_error(ec);
        return sock;
    }
} // namespace libwire::tcp
