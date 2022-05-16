#include "bit_ostream.h"

#include <cstdint>
#include <ostream>

BitOstream::BitOstream(std::ostream& out) : out_(out) {
    buffer_ = 0;
    ptr_ = 0;
}

BitOstream& BitOstream::flush() {
    if (ptr_ != 0) {
        out_.put(buffer_);
    }
    ptr_ = 0;
    buffer_ = 0;
    return (*this);
}

BitOstream& BitOstream::WriteOneBit(int16_t& bit) {
    buffer_ |= (bit << (BYTE_SIZE_ - ptr_ - 1));
    ++ptr_;
    if (ptr_ == BYTE_SIZE_) {
        flush();
    }
    return (*this);
}

BitOstream& BitOstream::write(int16_t var, size_t size) {
    for (size_t ind = 0; ind < size; ++ind) {
        int16_t bit = (var >> ind) & 1;
        WriteOneBit(bit);
    }
    return (*this);
}

BitOstream& BitOstream::write(size_t var, size_t size) {
    for (size_t ind = 0; ind < size; ++ind) {
        int16_t bit = (var >> ind) & 1;
        WriteOneBit(bit);
    }
    return (*this);
}

BitOstream& BitOstream::ReverseWrite(size_t var, size_t size) {
    for (size_t ind = 0; ind < size; ++ind) {
        int16_t bit = (var >> (size - ind - 1)) & 1;
        WriteOneBit(bit);
    }
    return (*this);
}

BitOstream& BitOstream::ReverseWrite(int16_t var, size_t size) {
    for (size_t ind = 0; ind < size; ++ind) {
        int16_t bit = (var >> (size - ind - 1)) & 1;
        WriteOneBit(bit);
    }
    return (*this);
}