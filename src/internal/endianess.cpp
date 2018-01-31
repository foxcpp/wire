#include "libwire/internal/endianess.hpp"

namespace libwire::internal_ {
    uint16_t flip_endianess(uint16_t input) noexcept {
        return (input >> 8u) | (input << 8u);
    }
    uint32_t flip_endianess(uint32_t input) noexcept {
        // clang-format off
        return (input >> 8u * 3u)                 |
               ((input << 8u * 1u) & 0x00FF0000u) |
               ((input >> 8u * 1u) & 0x0000FF00u) |
               (input << 8u * 3u);
        // clang-format on
    }

    bool host_is_network() noexcept {
        static const uint16_t probe = 1;
        // false if first byte is 1 (little endian).
        return *reinterpret_cast<const uint8_t*>(&probe) != 1;
    }
} // namespace libwire::internal_
