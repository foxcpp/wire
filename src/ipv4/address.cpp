#include <libwire/ipv4/address.hpp>
#include <algorithm>
#include <stdexcept>

// Homemade string-to-uint8_t because I really want to make
// libwire compilable and usable with -fno-exceptions while
// still having proper error handling.
uint8_t stou8(const std::string_view& str, bool& success) {
    // We write to a bigger buffer first so we
    // can catch overflow.
    if (str.empty()) {
        success = false;
        return false;
    }
    uint16_t buf = 0;
    for (auto ch : str) {
        if (!std::isdigit(ch)) {
            // Invalid character.
            success = false;
            return 0;
        }
        buf *= 10;
        buf += (ch - '0');
        if (buf > 255) {
            // Overflow.
            success = false;
            return 0;
        }
    }
    success = true;
    return uint8_t(buf);
}

namespace libwire::ipv4 {
    address::address(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {
        parts[0] = p1;
        parts[1] = p2;
        parts[2] = p3;
        parts[3] = p4;
    }


    address::address(const std::string_view& ip_string) {
        bool success;
        *this = address(ip_string, success);
#ifdef __cpp_exceptions
        if (!success) throw std::invalid_argument("Invalid IPv4 address string");
#endif
    }

    // Probaby safest way to implement parsing without exceptions, but
    // not fastest one. If somebody can implement fast & safe
    // c-tor that will pass all tests with -fno-exceptions - PR's are welcome!

    address::address(const std::string_view& ip_string, bool& success) {
        auto it = ip_string.cbegin();
        for (int i = 0; i < 4; ++i) {
            auto dot_it = std::find(it, ip_string.end(), '.');
            if (dot_it == ip_string.end() && i != 3) {
                success = false;
                return;
            }
            parts[i] = stou8(std::string{it, dot_it}, success);
            if (!success) {
                return;
            }
            it = dot_it;
            ++it;
        }
    }

    std::string address::to_string() const {
        std::string result;
        result.reserve(12);
        result += std::to_string(parts[0]);
        result += '.';
        result += std::to_string(parts[1]);
        result += '.';
        result += std::to_string(parts[2]);
        result += '.';
        result += std::to_string(parts[3]);
        return result;
    }

    bool address::operator==(const address& other) const {
        return parts[0] == other.parts[0] &&
               parts[1] == other.parts[1] &&
               parts[2] == other.parts[2] &&
               parts[3] == other.parts[3];
    }

    bool address::operator!=(const address& other) const {
        return parts[0] != other.parts[0] &&
               parts[1] != other.parts[1] &&
               parts[2] != other.parts[2] &&
               parts[3] != other.parts[3];
    }
} // namespace libwire::ipv4
