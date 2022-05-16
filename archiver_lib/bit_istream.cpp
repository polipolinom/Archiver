#include "bit_istream.h"

#include <cstdint>
#include <istream>

BitIstream::BitIstream(std::istream& in) : in_(in) {
    buffer_ = 0;
    ptr_ = BYTE_SIZE_;
}

BitIstream& BitIstream::ReadOneBit(int16_t& bit) {
    if (ptr_ >= BYTE_SIZE_) {
        ptr_ = 0;
        in_.get(buffer_);
    }
    bit = ((buffer_ >> (BYTE_SIZE_ - ptr_ - 1)) & 1);
    ++ptr_;
    return (*this);
}

BitIstream& BitIstream::read(int16_t& var, size_t size) {
    var = 0;
    size_t ptr = 0;
    for (size_t ind = 0; ind < size; ++ind) {
        int16_t bit = 0;
        ReadOneBit(bit);
        var ^= (bit << ptr);
        ++ptr;
    }
    return (*this);
}

BitIstream& BitIstream::read(size_t& var, size_t size) {
    var = 0;
    size_t ptr = 0;
    for (size_t ind = 0; ind < size; ++ind) {
        int16_t bit = 0;
        ReadOneBit(bit);
        var ^= (bit << ptr);
        ++ptr;
    }
    return (*this);
}

BitIstream::operator bool() const {
    return static_cast<bool>(in_);
}
