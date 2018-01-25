#include <iostream>
#include <libwire/tcp/socket.hpp>
#include <libwire/dns.hpp>

/**
 * \example echo_client.cpp
 *
 * This example shows how to implement simple echo server using libwire.
 *
 * 1. Resolve specified DNS name to set of IPv4 addresses, pick first
 *    and connect to it.
 *
 * 2. Read lines from stdin, write to socket, read string of matching
 *    length from socket and print it. Stop on EOF from stdin.
 */

int main(int argc, char** argv) {
    using namespace libwire;

    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: echo-client <ip> [port]\n";
        return 1;
    }

    uint16_t port = 7;
    if (argc == 3) {
        port = uint16_t(std::stoi(argv[2]));
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
