#include "libwire/internal/endianess.hpp"

namespace libwire::internal_ {
    uint16_t flip_endianess(uint16_t input) noexcept {
        return (input >> 8) | (input << 8);
    }
    uint32_t flip_endianess(uint32_t input) noexcept {
        return (input >> 8 * 3) | ((input << 8 * 1) & 0x00FF0000) | ((input >> 8 * 1) & 0x0000FF00) | (input << 8 * 3);
    }

    bool host_is_network() noexcept {
        static const uint16_t probe = 1;
        // false if first byte is 1 (little endian).
        return *reinterpret_cast<const uint8_t*>(&probe) != 1;
    }
} // namespace libwire::internal_
