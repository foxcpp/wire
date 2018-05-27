#include <iostream>
#include <libwire/udp.hpp>
#include <libwire/endpoint.hpp>

/**
 * \example echo_server.cpp
 *
 * This example shows how to implement TCP echo server using
 * \ref libwire::tcp::listener and \ref libwire::tcp::socket classes.
 *
 * Example code is pretty simple to understand but let's describe each
 * step in detail if you are in doubt:
 * 1. \code
 *    listener.listen(ipv4::any, port);
 *    \endcode
 *    Here we start listening on any IPv4-capable interface using specified
 * port.
 *
 * 2. Then we enter infinite loop, accept connection, echo received bytes until
 *    any error and continue with next connection.
 */

 constexpr size_t max_msg_size = 64;

int main(int argc, char** argv) {
    using namespace libwire;
    using namespace std::literals::string_view_literals;

    if (argc != 2) {
        std::cerr << "Usage: echo-server <port>\n";
        return 1;
    }

    uint16_t port = std::stoi(argv[1]);
    std::cout << "Listening on port " << port << ".\n";

    udp::socket sock(ip::v4);
    sock.listen({ipv4::loopback, port});

    endpoint source{{0, 0, 0, 0}, 0};
    std::string buf;
    while (true) {
        try {
            sock.read(max_msg_size, buf, &source);

            size_t lf = buf.find_last_of('\n');
            if (lf != buf.npos) {
                buf.resize(lf); // cut LF if present.
            }

            std::cout << "< " << buf << '\n';
            std::cout << "> " << buf << '\n';

            buf.push_back('\n');
            sock.write(buf, source);
        } catch (std::system_error& ex) {
            std::cout << "ERR: " << ex.what() << '\n';
        }
    }
}
