#include <libwire/address.hpp>
#include <cassert>
#include <algorithm>

#if __has_include(<unistd.h>)
    #include <unistd.h>
#endif

#ifdef _POSIX_VERSION
    #include <arpa/inet.h> // inet_pton on POSIX systems
#endif


namespace libwire {
    address::address(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4)
        : version(ip::v4)
        , parts{o1,  o2,  o3,  o4,  0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0} {}

    address::address(uint8_t o1,  uint8_t o2,  uint8_t o3,  uint8_t o4,
                     uint8_t o5,  uint8_t o6,  uint8_t o7,  uint8_t o8,
                     uint8_t o9,  uint8_t o10, uint8_t o11, uint8_t o12,
                     uint8_t o13, uint8_t o14, uint8_t o15, uint8_t o16)
        : version(ip::v6)
        , parts{o1, o2,  o3,  o4,  o5,  o6,  o7,  o8,
                o9, o10, o11, o12, o13, o14, o15, o16} {
    }

    address::address(const std::string_view& sv) {
        bool success = true;
        *this = address(sv, success);
        if (!success) throw std::invalid_argument("Invalid address string");
    }

    address::address(const std::string_view& sv, bool& success) {
#ifdef _POSIX_VERSION
        int family = AF_INET;
        version = ip::v4;
        for (const char& ch : sv) {
            if (ch == ':') {
                family = AF_INET6;
                version = ip::v6;
            }
        }

        success = inet_pton(family, sv.data(), parts.data());
        // ^ returns 1 on success, 0 otherwise.

        // For IPv4 we need to zero-rewrite parts not
        // set by inet_pton.
        if (family == AF_INET) {
            std::fill(parts.begin() + sizeof(in_addr), parts.end(), 0x00);
        }
#else
    #error "libwire doesn't supports your platform. :("
#endif
    }

    std::string address::to_string() const {
#ifdef _POSIX_VERSION
        int family = (version == ip::v4) ? AF_INET : AF_INET6;

        std::array<char, 45> buffer;

        const char* buf_ptr = inet_ntop(family, parts.data(),
                                        buffer.data(), buffer.size());
        assert(buf_ptr != 0); // This function generally should not fail.

        return std::string(buffer.data());
#else
    #error "libwire doesn't supports your platform. :("
#endif
    }

    bool address::operator==(const address& o) const {
        return version == o.version && parts == o.parts;
    }

    bool address::operator!=(const address& o) const {
        return version == o.version && parts != o.parts;
    }
} // namespace libwire
