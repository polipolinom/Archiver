#ifndef CPP_PILOT_HSE_BIT_OSTREAM_H
#define CPP_PILOT_HSE_BIT_OSTREAM_H

#include <cstdint>
#include <ostream>

class BitOstream{
public:
    BitOstream(std::ostream& out);
    BitOstream& flush();
    BitOstream& write(int16_t var, size_t size);
    BitOstream& write(size_t var, size_t size);
    BitOstream& ReverseWrite(int16_t var, size_t size);
    BitOstream& ReverseWrite(size_t var, size_t size);

private:
    const int BYTE_SIZE_ = 8;
    std::ostream& out_;
    char buffer_ = 0;
    size_t ptr_ = BYTE_SIZE_;

    BitOstream& WriteOneBit(int16_t& bit);
};

#endif  // CPP_PILOT_HSE_BIT_OSTREAM_H
