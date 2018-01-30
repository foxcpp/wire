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

#include "gtest.hpp"
#include <libwire/memory_view.hpp>

TEST(MemoryView, NullState) {
    libwire::memory_view view;
    ASSERT_EQ(view.capacity(), 0);
    ASSERT_EQ(view.size(), 0);
    ASSERT_EQ(view.max_size(), 0);
    ASSERT_EQ(view.data(), nullptr);
#ifdef __cpp_exceptions
    ASSERT_THROW(view.at(7), std::out_of_range);
#endif
    // Typical guarantee for empty containers.
    ASSERT_EQ(view.begin(), view.end());
}

TEST(MemoryView, AssignFromArray) {
    uint8_t test[15] = "Hello, libwire";
    libwire::memory_view view(test, 15);

    ASSERT_EQ(view.data(), test);

    ASSERT_EQ(view.size(), 15);
    ASSERT_EQ(view.capacity(), 15);
    ASSERT_EQ(view.max_size(), 15);
    ASSERT_EQ(std::distance(view.begin(), view.end()), 15);

    int i = 0;
    for (auto it = view.begin(); it != view.end(); ++it, ++i) {
        ASSERT_EQ(view[i], *it);
        ASSERT_EQ(view[i], test[i]);
    }
}

TEST(MemoryView, ShrinkBack) {
    uint8_t test[15] = "Hello, libwire";
    libwire::memory_view view(test, 15);

    view.shrink_back(5);

    ASSERT_EQ(view.size(), 15 - 5);
    ASSERT_EQ(view.capacity(), 15);
    ASSERT_EQ(view.max_size(), 15);

    ASSERT_EQ(view.back(), test[15 - 5 - 1]);
}

TEST(MemoryView, ShrinkFront) {
    uint8_t test[15] = "Hello, libwire";
    libwire::memory_view view(test, 15);

    view.shrink_front(5);

    ASSERT_EQ(view.size(), 15 - 5);
    ASSERT_EQ(view.capacity(), 10);
    ASSERT_EQ(view.max_size(), 10);

    ASSERT_EQ(view.front(), test[5]);
}

TEST(MemoryView, Clear) {
    uint8_t test[15] = "Hello, libwire";
    libwire::memory_view view(test, 15);

    view.clear();

    ASSERT_EQ(view.size(), 0);
    ASSERT_EQ(view.capacity(), 15);
    ASSERT_EQ(view.max_size(), 15);
}

TEST(MemoryView, Resize) {
    uint8_t test[15] = "Hello, libwire";
    libwire::memory_view view(test, 15);

    view.resize(5);

    ASSERT_EQ(view.size(), 5);
    ASSERT_EQ(view.capacity(), 15);
    ASSERT_EQ(view.max_size(), 15);

#ifdef __cpp_exceptions
    ASSERT_THROW(view.resize(view.capacity() + 5), std::out_of_range);
#endif
}
