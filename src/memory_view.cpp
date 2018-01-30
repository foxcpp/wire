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

#include "libwire/memory_view.hpp"
#include <stdexcept>

namespace libwire {
    memory_view::memory_view() noexcept : data_(nullptr), size_(0), capacity_(0) {
    }

    memory_view::memory_view(void* memory, memory_view::size_type size_bytes) noexcept
        : data_(reinterpret_cast<uint8_t*>(memory)), size_(size_bytes), capacity_(size_bytes) {
    }

#ifdef __cpp_exceptions
    memory_view::const_reference memory_view::at(memory_view::size_type i) const {
        if (i > size()) {
            throw std::out_of_range("Index is bigger than size");
        }
        return *(data_ + i);
    }
#endif // ifdef __cpp_exceptions

    memory_view::const_reference memory_view::operator[](memory_view::size_type i) const noexcept {
        return *(data_ + i);
    }

#ifdef __cpp_exceptions
    memory_view::reference memory_view::at(memory_view::size_type i) {
        if (i > size()) {
            throw std::out_of_range("Index is bigger than size");
        }
        return *(data_ + i);
    }
#endif // ifdef __cpp_exceptions

    memory_view::reference memory_view::operator[](memory_view::size_type i) noexcept {
        return *(data_ + i);
    }

    memory_view::const_reference memory_view::front() const noexcept {
        return *data_;
    }

    memory_view::const_reference memory_view::back() const noexcept {
        return *(data_ + size() - 1);
    }

    memory_view::reference memory_view::front() noexcept {
        return *data_;
    }

    memory_view::reference memory_view::back() noexcept {
        return *(data_ + size() - 1);
    }

    memory_view::const_pointer memory_view::data() const noexcept {
        return data_;
    }

    memory_view::pointer memory_view::data() noexcept {
        return data_;
    }

    memory_view::const_iterator memory_view::begin() const noexcept {
        return data_;
    }

    memory_view::const_iterator memory_view::end() const noexcept {
        return data_ + size();
    }

    memory_view::iterator memory_view::begin() noexcept {
        return data_;
    }

    memory_view::iterator memory_view::end() noexcept {
        return data_ + size();
    }

    memory_view::const_iterator memory_view::cbegin() const noexcept {
        return data_;
    }

    memory_view::const_iterator memory_view::cend() const noexcept {
        return data_ + size();
    }

    memory_view::size_type memory_view::size() const noexcept {
        return size_;
    }

    memory_view::size_type memory_view::max_size() const noexcept {
        return capacity_;
    }

    memory_view::size_type memory_view::capacity() const noexcept {
        return capacity_;
    }

    void memory_view::shrink_back(memory_view::size_type bytes_count) noexcept {
        size_ -= bytes_count;
    }

    void memory_view::shrink_front(memory_view::size_type bytes_count) noexcept {
        capacity_ -= bytes_count;
        size_ -= bytes_count;
        data_ += bytes_count;
    }

    void memory_view::clear() noexcept {
        size_ = 0;
    }

    void memory_view::resize(size_t new_size) noexcept(!LIBWIRE_EXCEPTIONS_ENABLED_BOOL) {
        if (new_size > capacity_) {
#ifdef __cpp_exceptions
            throw std::out_of_range("Too big new_size");
#else
            return;
#endif
        }
        size_ = new_size;
    }

    void memory_view::swap(memory_view& other) noexcept {
        using std::swap;

        std::swap(this->data_, other.data_);
        std::swap(this->size_, other.size_);
        std::swap(this->capacity_, other.capacity_);
    }
} // namespace libwire
