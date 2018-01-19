#include <iostream>
#include <libwire/tcp/socket.hpp>
#include <libwire/dns.hpp>

/**
 * This example shows how to implement simple echo server
 * using libwire.
 *
 * Note: Due to very early state of library development
 * this example doesn't supports IPv6 and DNS name resolution.
 * Also, error handling seems to be verbose because no
 * exceptions used.
 */

void report_error(const std::string_view& source, std::error_code ec) {
    std::cerr << source << ": " << ec.message() << " (" << ec.value() << ")\n";
    std::exit(2);
}

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

    std::error_code ec;

    tcp::socket socket;
    socket.connect(dns::resolve(ip::v4, argv[1], ec).at(0), port, ec);
    if (ec) report_error ("connect", ec);

    std::string buffer;

    while (std::cout << "> ", std::getline(std::cin, buffer)) {
        socket.write(buffer, ec);
        if (ec) report_error ("write", ec);

        socket.read(buffer.size(), buffer, ec);
        if (ec) report_error ("read", ec);

        std::cout << "< " << buffer << '\n';
    }
}
