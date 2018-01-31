#include <libwire/address.hpp>
#include <cassert>
#include <algorithm>

#if __has_include(<unistd.h>)
#    include <unistd.h>
#endif

#ifdef _POSIX_VERSION
#    include <arpa/inet.h> // inet_pton on POSIX systems
#endif

namespace libwire {
    address::address(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4) noexcept
        : version(ip::v4), parts{o1, o2, o3, o4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0} {
    }

    address::address(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4, uint8_t o5, uint8_t o6, uint8_t o7, uint8_t o8,
                     uint8_t o9, uint8_t o10, uint8_t o11, uint8_t o12, uint8_t o13, uint8_t o14, uint8_t o15,
                     uint8_t o16) noexcept
        : version(ip::v6), parts{o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16} {
    }

    address::address(const std::string_view& text_ip) { // NOLINT(hicpp-member-init)
        bool success = true;
        *this = address(text_ip, success);
        if (!success) throw std::invalid_argument("Invalid address string");
    }

    address::address(const std::string_view& text_ip, bool& success) noexcept : version(ip::v4), parts{} {
#ifdef _POSIX_VERSION
        int family = AF_INET;
        for (const char& ch : text_ip) {
            if (ch == ':') {
                family = AF_INET6;
                version = ip::v6;
            }
        }

        success = bool(inet_pton(family, text_ip.data(), parts.data()));
        // ^ returns 1 on success, 0 otherwise.
#else
#    error "libwire doesn't supports your platform. :("
#endif
    }

    std::string address::to_string() const noexcept {
#ifdef _POSIX_VERSION
        int family = (version == ip::v4) ? AF_INET : AF_INET6;

        std::array<char, 45> buffer;

        const char* buf_ptr = inet_ntop(family, parts.data(), buffer.data(), buffer.size());
        assert(buf_ptr != nullptr); // This function generally should not fail.

        return std::string(buffer.data());
#else
#    error "libwire doesn't supports your platform. :("
#endif
    }

    bool address::operator==(const address& o) const noexcept {
        return version == o.version && parts == o.parts;
    }

    bool address::operator!=(const address& o) const noexcept {
        return version == o.version && parts != o.parts;
    }

    address::address(const memory_view& mv) noexcept {
        assert(mv.size() == 4 || mv.size() == 16);

        version = mv.size() == 4 ? ip::v4 : ip::v6;
        std::copy(mv.begin(), mv.end(), parts.begin());
    }
} // namespace libwire

namespace std { // NOLINT(cert-dcl58-cpp)
    std::size_t hash<libwire::address>::operator()(const libwire::address& addr) const noexcept {
        std::hash<uint8_t> hash;
        auto result = std::size_t(addr.version);
        for (const uint8_t& i : addr.parts) {
            // See
            // https://stackoverflow.com/questions/4948780/magic-number-in-boosthash-combine
            result ^= hash(i) + 0x9e3779b9u + (result << 6u) + (result >> 2u);
        }
        return result;
    }
} // namespace std
