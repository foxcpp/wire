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

#include <thread>
#include <chrono>
#include "../gtest.hpp"
#include <libwire/tcp.hpp>

using namespace std::literals::chrono_literals;

static uint16_t port_to_use = 7777;

using namespace libwire;

struct TcpSocketPair : testing::TestWithParam<address> {
    void SetUp() override {
        listener.listen({GetParam(), port_to_use});
        std::thread connect_thr([&]() {
            std::this_thread::sleep_for(100ms);
            client.connect({GetParam(), port_to_use});
        });

        server = listener.accept();
        if (connect_thr.joinable()) connect_thr.join();

        // This will allow us to rerun tests without waiting for
        // TIME_WAIT (2*msl). Magic but works.
        server.set_option(tcp::linger, true, 0s);
        client.set_option(tcp::linger, true, 0s);
    }

    void TearDown() override {
        if (client.is_open()) client.shutdown();
        if (server.is_open()) server.shutdown();
        listener = tcp::listener();
    }

    tcp::listener listener;
    tcp::socket server, client;
};

TEST_P(TcpSocketPair, Connect) {
    ASSERT_TRUE(server.is_open());
    ASSERT_TRUE(client.is_open());
}

TEST_P(TcpSocketPair, EndpointsConsistency) {
    ASSERT_EQ(client.remote_endpoint(), endpoint(GetParam(), 7777));
    ASSERT_EQ(client.local_endpoint().addr, GetParam());
    ASSERT_EQ(server.local_endpoint().addr, GetParam());
    ASSERT_EQ(client.remote_endpoint(), server.local_endpoint());
    ASSERT_EQ(server.remote_endpoint(), client.local_endpoint());
}

TEST_P(TcpSocketPair, BasicIntegrityCheck) {
    for (unsigned i = 0; i < 10; ++i) {
        auto vec = std::vector<uint8_t>(1024 * (i + 1), 0x00);

        client.write(vec);
        auto vec2 = server.read(vec.size());
        ASSERT_EQ(vec, vec2);
    }
}

TEST_P(TcpSocketPair, ReadUntilIntegrityCheck) {
    for (unsigned i = 0; i < 10; ++i) {
        auto vec = std::vector<uint8_t>(1024 * (i + 1), 0x00);
        vec.push_back(0xFF);

        client.write(vec);
        auto vec2 = server.read_until(0xFF, vec);
        ASSERT_EQ(vec, vec2);
    }
}

INSTANTIATE_TEST_CASE_P(Ipv4, TcpSocketPair, ::testing::Values(ipv4::loopback));
INSTANTIATE_TEST_CASE_P(Ipv6, TcpSocketPair, ::testing::Values(ipv6::loopback));

TEST(TcpSocket, Errors) {
    std::error_code ec;
    {
        tcp::socket socket;

        // XXX: This can fail if we actually have something on 65500 (and we can
        // have because this is dynamic port!!!).
        socket.connect({ipv4::loopback, 65500}, ec);
        ASSERT_TRUE(ec);
        ASSERT_EQ(ec, error::connection_refused);
    }
}
