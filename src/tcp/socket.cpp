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

    socket::~socket() {
        open = false;
        shutdown();
        close();
    }

    internal_::socket::native_handle_t socket::native_handle() const noexcept {
        return implementation.native_handle();
    }

    bool socket::is_open() const {
        return this->open;
    }

    void socket::connect(address target, uint16_t port, std::error_code& ec) noexcept {
        implementation = internal_::socket(target.version, transport::tcp, ec);
        if (ec) return;
        implementation.connect(target, port, ec);
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

#ifdef __cpp_exceptions
    void socket::connect(address target, uint16_t port) {
        std::error_code ec;
        connect(target, port, ec);
        if (ec) throw std::system_error(ec);
    }

    std::tuple<address, uint16_t> socket::local_endpoint() const noexcept {
        return implementation.local_endpoint();
    }

    std::tuple<address, uint16_t> socket::remote_endpoint() const noexcept {
        return implementation.remote_endpoint();
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
