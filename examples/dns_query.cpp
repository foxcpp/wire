#include <iostream>
#include <libwire/dns.hpp>

/**
 * \example dns_query.cpp
 *
 * This super-simple example shows how to use \ref libwire::dns namespace
 * functions to query name-address mappings from system resolver.
 */

int main(int argc, char** argv) {
    using namespace libwire;

    if (argc != 2) {
        std::cerr << "Usage: dns-query <domain>\n";
        return 1;
    }

    std::vector<address> result_v4;
    try {
        result_v4 = dns::resolve(ip::v4, argv[1]);
    } catch (std::system_error& ec) {
        std::cerr << "Failed to query IPv4 info: " << ec.what() << '\n';
    }

    if (result_v4.empty()) {
        std::cout << "No IPv4 results.\n";
    } else {
        std::cout << "IPv4:\n";
    }
    for (const auto& address : result_v4) {
        std::cout << "    " << address.to_string() << '\n';
    }

    std::cout << '\n';

    std::vector<address> result_v6;
    try {
        result_v6 = dns::resolve(ip::v6, argv[1]);
    } catch (std::system_error& ec) {
        std::cerr << "Failed to query IPv6 info: " << ec.what() << '\n';
    }

    if (result_v6.empty()) {
        std::cout << "No IPv6 results.\n";
    } else {
        std::cout << "IPv6:\n";
    }
    for (const auto& address : result_v6) {
        std::cout << "    " << address.to_string() << '\n';
    }
}
