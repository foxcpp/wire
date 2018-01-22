#include <iostream>
#include <libwire/tcp/socket.hpp>
#include <libwire/dns.hpp>

/**
 * This example shows how to implement simple echo server
 * using libwire.
 */

int main(int argc, char** argv) {
    using namespace libwire;

    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: echo-client <ip> [port]\n";
        return 1;
    }

    uint16_t port = 7;
    if (argc == 3) {
        port = std::stoi(argv[2]);
    }

    tcp::socket socket;
    socket.connect(dns::resolve(ip::v4, argv[1]).at(0), port);

    std::string buffer;

    while (std::cout << "> ", std::getline(std::cin, buffer)) {
        socket.write(buffer);
        socket.read(buffer.size(), buffer);

        std::cout << "< " << buffer << '\n';
    }
}
