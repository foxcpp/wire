#include <iostream>
#include <libwire/tcp/listener.hpp>

/**
 * Example of tcp::listener class usage for echo server.
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

        std::cout << "Accepted connection.\n";

        while (true) {
            sock.read(1, buf);
            sock.write(buf);
        }

        std::cout << "Disconnected.\n";
    }
}
