#include <iostream>
#include <libwire/tcp/listener.hpp>

/**
 * Example of tcp::listener class usage for echo server.
 */

void report_error(const std::string_view& source, std::error_code ec) {
    std::cerr << source << ": " << ec.message() << " (" << ec.value() << ")\n";
    std::exit(2);
}

int main(int argc, char** argv) {
    using namespace libwire;

    if (argc != 2) {
        std::cerr << "Usage: echo-server <port>\n";
        return 1;
    }

    uint16_t port = std::stoi(argv[1]);

    std::error_code ec;

    tcp::listener listener;
    listener.listen(ipv4::any, port, ec);
    if (ec) report_error("listen", ec);

    std::cout << "Listening on port " << port << ".\n";

    std::string buf;
    while (true) {
        auto sock = listener.accept(ec);
        if (ec) {
            std::cerr << "accept" << ": " << ec.message() << " (" << ec.value() << ")\n";
            continue;
        }

        std::cout << "Accepted connection.\n";

        while (!ec) {
            sock.read(1, buf, ec);
            if (ec) continue;
            sock.write(buf, ec);
        }
        std::cout << "Disconnected.\n";
        ec = std::error_code();
    }
}
