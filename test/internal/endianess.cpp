#include <gtest/gtest.h>
#include "libwire/internal/endianess.hpp"

TEST(ImplEndianess, Uint16Flip) {
    using namespace libwire::internal_;

    ASSERT_EQ(flip_endianess(uint16_t(7)), 1792);
    ASSERT_EQ(flip_endianess(uint16_t(1792)), 7);
    ASSERT_EQ(flip_endianess(flip_endianess(uint16_t(65535))), 65535);
}

TEST(ImplEndianess, Uint32Flip) {
    using namespace libwire::internal_;

    ASSERT_EQ(flip_endianess(uint32_t(7)), 117440512);
    ASSERT_EQ(flip_endianess(uint32_t(117440512)), 7);
    ASSERT_EQ(flip_endianess(flip_endianess(uint32_t(65535))), 65535);
}

TEST(ImplEndianess, LittleEndianDetect) {
    using namespace libwire::internal_;

    bool big_endian = (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__);
    ASSERT_EQ(big_endian, host_is_network());

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
