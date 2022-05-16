#ifndef CPP_PILOT_HSE_BIT_ISTREAM_H
#define CPP_PILOT_HSE_BIT_ISTREAM_H

#include <cstdint>
#include <istream>

class BitIstream {
public:
    BitIstream(std::istream& in);
    BitIstream& read(int16_t& var, size_t size);
    BitIstream& read(size_t& var, size_t size);
    operator bool() const;

private:
    const int BYTE_SIZE_ = 8;
    std::istream& in_;
    char buffer_ = 0;
    size_t ptr_ = BYTE_SIZE_;

    BitIstream& ReadOneBit(int16_t& bit);
};

#endif  // CPP_PILOT_HSE_BIT_ISTREAM_H
