#ifndef CPP_PILOT_HSE_ARCHIVER_H
#define CPP_PILOT_HSE_ARCHIVER_H

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "bit_ostream.h"

void Archive(std::istream& in, std::ostream& out, const std::string& filename, bool is_end = false);

void GetCanonical(std::vector<std::pair<size_t, int16_t>>& huffman_code,
                  std::vector<std::pair<size_t, size_t>>& huffman_code_size);

#endif  // CPP_PILOT_HSE_ARCHIVER_H
