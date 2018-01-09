#include <cstring>
#include <iostream>
#include <libwire/impl/posix_socket.hpp>

/**
 * Sometimes you may want to use sockets directly while still having
 * RAII-style handle for it. libwire provides such type but note one
 * important thing: this is implementation detail and not a part of
 * public API so it can be changed anytime.
 *
 * Warning: This type is availible only on some POSIX platforms.
 */

int main() {
    using namespace libwire::impl_;

    std::error_code ec;

    // Allocate new socket for TCP/IPv4 connection.
    posix_socket sock(posix_socket::v4, posix_socket::tcp, ec);
    if (ec) {
        std::cerr << "posix_socket::posix_socket: " << ec.message() << '\n';
        return 1;
    }

    // Connect to local TCP server running on 7 port (echo protocol).
    sock.connect({127, 0, 0, 1}, 7, ec);
    if (ec) {
        std::cerr << "posix_socket::connect: " << ec.message() << '\n';
        return 1;
    }

    // Write contents of C-string to socket.
    const char* hello = "Hello, libwire!";
    sock.write(hello, strlen(hello), ec);
    if (ec) {
        std::cerr << "posix_socket::write: " << ec.message() << '\n';
        return 1;
    }

    // Read 16 byte from socket to C-array buffer.
    char output_buffer[256];
    sock.read(output_buffer, strlen(hello), ec);
    if (ec) {
        std::cerr << "posix_socket::read: " << ec.message() << '\n';
        return 1;
    }

    std::cout << output_buffer << '\n';

    // Socket closed by destructor here. You don't have to worry about it.
}
