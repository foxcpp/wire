#include <iostream>
#include <libwire/tcp/listener.hpp>

/**
 * \example echo_server.cpp
 *
 * This example shows how to implement TCP echo server using
 * \ref tcp::listener and \ref tcp::socket classes.
 *
 * Example code is pretty simple to understand but let's describe each
 * step in detail if you are in doubt:
 * 1. \code
 *    listener.listen(ipv4::any, port);
 *    \endcode
 *    Here we start listening on any IPv4-capable interface using specified port.
 *
 * 2. Then we enter infinite loop, accept connection, echo received bytes until
 *    any error and continue with next connection.
 */

int main(int argc, char** argv) {
    using namespace libwire;

    if (argc != 2) {
        std::cerr << "Usage: echo-server <port>\n";
        return 1;
    }

    uint16_t port = std::stoi(argv[1]);

    tcp::listener listener;
    listener.listen(ipv4::any, port);

    std::cout << "Listening on port " << port << ".\n";

    std::string buf;
    while (true) {
        auto sock = listener.accept();
        std::tuple source = sock.remote_endpoint();

        std::cout << "Accepted connection from "
                  << std::get<0>(source).to_string() << ':' << std::get<1>(source)  << ".\n";

        while (true) {
            try {
                sock.read(1, buf);
                sock.write(buf);
            } catch (std::system_error& ) {
                break;
            }
        }

        std::cout << "Disconnected.\n";
    }
}
