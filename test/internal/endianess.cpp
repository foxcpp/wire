/*
 * Copyright © 2018 Max Mazurov (fox.cpp) <fox.cpp [at] disroot [dot] org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../gtest.hpp"
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
