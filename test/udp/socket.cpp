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
#include <libwire/udp.hpp>
#include <libwire/options.hpp>

using namespace libwire;
static uint16_t port_to_use = 7777;

TEST(UDPSocket, Integrity) {
    // UDP is unreliable protocol however we assume that
    // communication over loopback is safe.
    // This tests checks the most trivial case of operation.

    udp::socket receiver(ip::v4), sender(ip::v4);
    receiver.listen({ipv4::loopback, port_to_use});

    std::vector<uint8_t> buffer(128, 0xEF);
    sender.write(buffer, {ipv4::loopback, port_to_use});
    std::vector<uint8_t> buffer2 = receiver.read(buffer.size());
    ASSERT_EQ(buffer, buffer2);
}

TEST(UDPSocket, TruncatedDatagram) {
    // Here we check if datagram is correctly truncated
    // when receiver buffer is too small.

    udp::socket receiver(ip::v4), sender(ip::v4);
    receiver.listen({ipv4::loopback, port_to_use});

    std::vector<uint8_t> buffer(128, 0xEF);
    buffer.push_back(0xFF); buffer.push_back(0xFF); buffer.push_back(0xFF);
    sender.write(buffer, {ipv4::loopback, port_to_use});

    // Sender sent 131 bytes but we want only 128, documentation says that we
    // should receive only 128 bytes and discard remaining.
    std::vector<uint8_t> buffer2 = receiver.read(128);
    ASSERT_EQ(buffer2.size(), 128);
    buffer.resize(128);
    ASSERT_EQ(buffer2, buffer);
}

TEST(UDPSocket, TooSmallDatagram) {
    // If datagram is too small - buffer should be resized to it's size.

    udp::socket receiver(ip::v4), sender(ip::v4);
    receiver.listen({ipv4::loopback, port_to_use});

    std::vector<uint8_t> buffer(64, 0xEF);
    sender.write(buffer, {ipv4::loopback, port_to_use});

    // Ouch! Receiver wants 128 but we sent only 64.
    std::vector<uint8_t> buffer2 = receiver.read(128);
    ASSERT_EQ(buffer2.size(), 64);
}

TEST(UDPSocket, NoAssociationError) {
    // We should get error if there is no destination set.

    udp::socket sender(ip::v4);
    // Ouch! No association and no explicit destination
    ASSERT_THROW(sender.write(std::vector<uint8_t>{1,2,3,4}), std::system_error);
}

TEST(UDPSocket, Association) {
    // In previous tests we didn't used association because
    // it may be broken... Let's see if everything is fine.

    udp::socket receiver(ip::v4), sender(ip::v4);
    receiver.listen({ipv4::loopback, port_to_use});

    sender.associate({ipv4::loopback, port_to_use});
    // No explicit destination but association present.
    sender.write(std::vector<uint8_t>{1,2,3,4});

    // This will prevent hanging if datagram is lost.
    receiver.set_option(non_blocking, true);

    std::vector<uint8_t> buf = receiver.read(4);
    ASSERT_EQ(buf.size(), 4);
}

TEST(UDPSocket, Disassociate) {
    udp::socket receiver(ip::v4), sender(ip::v4);
    receiver.listen({ipv4::loopback, port_to_use});

    sender.associate({ipv4::loopback, port_to_use});
    sender.disassociate();
    // Ouch! No association and no explicit destination
    ASSERT_THROW(sender.write(std::vector<uint8_t>{1,2,3,4}), std::system_error);
}