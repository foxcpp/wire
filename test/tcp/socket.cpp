/*
 * Copyright © 2018 Maks Mazurov (fox.cpp) <foxcpp [at] yandex [dot] ru>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
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
#include <libwire/tcp/socket.hpp>
#include <libwire/tcp/listener.hpp>

using namespace libwire;
using namespace std::literals::chrono_literals;

static uint16_t port_to_use = 7777;

/**
 * Create two connected sockets for testing.
 */
static std::tuple<tcp::socket, tcp::socket> socket_pair() {
    ++port_to_use;

    tcp::socket server, client;
    tcp::listener listener{ipv4::loopback, port_to_use};

    std::thread connect_thr([&]() {
        std::this_thread::sleep_for(100ms);
        client.connect(ipv4::loopback, port_to_use);
    });

    server = listener.accept();
    if (connect_thr.joinable()) connect_thr.join();

    return {std::move(server), std::move(client)};
}

static std::vector<uint8_t> random_vector(size_t size) {
    std::vector<uint8_t> vec(size);
    std::generate(vec.begin(), vec.end(), std::rand); // NOLINT(cert-msc30-c,cert-msc50-cpp)
    return vec;
}

TEST(TcpSocket, Connect) {
    auto [ server, client ] = socket_pair();
    ASSERT_TRUE(server.is_open());
    ASSERT_TRUE(client.is_open());
}

TEST(TcpSocket, BasicIntegrityCheck) {
    std::srand(std::time(nullptr));
    for (unsigned i = 0; i < 10; ++i) {
        auto vec = random_vector(1024 * std::rand() % 1024); // NOLINT(cert-msc30-c,cert-msc50-cpp)
        auto [ server, client ] = socket_pair();

        client.write(vec);
        auto vec2 = server.read(vec.size());
        ASSERT_EQ(vec, vec2);
        client.shutdown();
        server.shutdown();
    }
}

// TCP state machine is broken here, if we test
// it from separate threads then it will cause Broken Pipe
// on write as it should, but exactly here it doesn't works for
// some reason.

//TEST(TcpSocker, CloseOnFailure) {
//    auto vec = random_vector(512);
//    auto [ server, client ] = socket_pair();
//    server.close();
//    ASSERT_FALSE(server.is_open());
//    client.write(vec);
//    ASSERT_FALSE(client.is_open());
//}

