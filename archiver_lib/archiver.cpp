#include "archiver.h"

#include <algorithm>
#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "binary_trie.h"
#include "bit_ostream.h"
#include "priority_queue.h"

namespace {
using CodeTable = std::vector<std::pair<size_t, int16_t>>;
using SizeTable = std::vector<std::pair<size_t, size_t>>;
using SymbolTable = std::vector<std::pair<int16_t, size_t>>;

const size_t FILENAME_END = 256;
const size_t ONE_MORE_FILE = 257;
const size_t ARCHIVE_END = 258;

const size_t MAX_SYMBOL = 256;
const int OUT_SIZE = 9;
const int MAX_CODE_SIZE = 16;
}

size_t CharToSizeT(char symbol) {
    return (symbol + MAX_SYMBOL) % MAX_SYMBOL;
}

std::vector<size_t> GetFrequency(std::string filename, std::istream& in, bool is_end) {
    std::vector<size_t> frequency(MAX_SYMBOL + 3);
    for (const auto& symbol : filename) {
        ++frequency[CharToSizeT(symbol)];
    }
    char symbol;
    while (in.read(&symbol, sizeof(unsigned char))) {
        ++frequency[CharToSizeT(symbol)];
    }
    ++frequency[FILENAME_END];
    if (is_end == true) {
        ++frequency[ARCHIVE_END];
    } else {
        ++frequency[ONE_MORE_FILE];
    }
    return frequency;
}

CodeTable GetHuffmanCode(std::vector<size_t>& frequency) {
    PriorityQueue current_tries;
    for (size_t symbol = 0; symbol < frequency.size(); ++symbol) {
        if (frequency[symbol] > 0) {
            current_tries.push({symbol, frequency[symbol]});
        }
    }
    if (current_tries.size() == 0) {
        return {};
    }
    while (current_tries.size() > 1) {
        auto first_trie = current_tries.front();
        current_tries.pop();
        auto second_trie = current_tries.front();
        current_tries.pop();
        BinaryTrie new_trie(first_trie, second_trie);
        current_tries.push(new_trie);
    }
    return current_tries.front().GetBinaryCodes();
}

size_t CountBits(int16_t number) {
    size_t count = 0;
    while (number > 1) {
        ++count;
        number >>= 1;
    }
    return count;
}

bool CmpForSizes(const std::pair<size_t, size_t>& a, const std::pair<size_t, size_t>& b) {
    if (a.second != b.second) {
        return a.second < b.second;
    }
    return a.first < b.first;
}

void GetCanonical(CodeTable& huffman_code, SizeTable& huffman_code_size) {
    std::sort(huffman_code_size.begin(), huffman_code_size.end(), CmpForSizes);
    int16_t prefix = 0;
    size_t prefix_len = 1;
    for (size_t ind = 0; ind < huffman_code_size.size(); ++ind) {
        while (prefix_len < huffman_code_size[ind].second) {
            prefix <<= 1;
            prefix_len++;
        }
        huffman_code[ind] = {huffman_code_size[ind].first, prefix};
        prefix++;
        if (prefix >= (1 << (prefix_len))) {
            prefix_len++;
        }
    }
}

SizeTable GetSizeTables(const CodeTable& codes) {
    SizeTable sizes;
    for (const auto& [symbol, code] : codes) {
        sizes.push_back({symbol, CountBits(code)});
    }
    return sizes;
}

void Normalized(CodeTable& codes) {
    for (auto& [symbol, code] : codes) {
        code ^= (1 << CountBits(code));
    }
}

void PrintText(const std::string& filename, std::istream& in, BitOstream& out, bool is_end,
               SymbolTable codes) {
    in.clear();
    in.seekg(0);
    for (const auto& symbol : filename) {
        out.ReverseWrite(codes[CharToSizeT(symbol)].first, codes[CharToSizeT(symbol)].second);
    }
    out.ReverseWrite(codes[FILENAME_END].first, codes[FILENAME_END].second);
    char symbol;
    while (in.read(&symbol, sizeof(unsigned char))) {
        out.ReverseWrite(codes[CharToSizeT(symbol)].first, codes[CharToSizeT(symbol)].second);
    }
    if (is_end == true) {
        out.ReverseWrite(codes[ARCHIVE_END].first, codes[ARCHIVE_END].second);
    } else {
        out.ReverseWrite(codes[ONE_MORE_FILE].first, codes[ONE_MORE_FILE].second);
    }
}

void Print(BitOstream& out, const std::string& filename, std::istream& in, bool is_end,
           SizeTable huffman_code_size, SymbolTable codes) {
    out.write(huffman_code_size.size(), OUT_SIZE);
    for (const auto& [symbol, code] : huffman_code_size) {
        out.write(symbol, OUT_SIZE);
    }
    size_t ind = 0;
    for (size_t symbol_code_size = 1; symbol_code_size <= MAX_CODE_SIZE; ++symbol_code_size) {
        size_t count = 0;
        while (ind < huffman_code_size.size() && huffman_code_size[ind].second == symbol_code_size) {
            ++ind;
            ++count;
        }
        out.write(count, OUT_SIZE);
        if (ind == huffman_code_size.size()) {
            break;
        }
    }
    PrintText(filename, in, out, is_end, codes);
    out.flush();
}

void Archive(std::istream& in, std::ostream& out, const std::string& filename, bool is_end) {
    BitOstream bit_out(out);
    auto frequency = GetFrequency(filename, in, is_end);
    auto huffman_code = GetHuffmanCode(frequency);
    auto huffman_code_size = GetSizeTables(huffman_code);
    Normalized(huffman_code);
    GetCanonical(huffman_code, huffman_code_size);
    SymbolTable codes(MAX_SYMBOL + 3);
    for (size_t ind = 0; ind < huffman_code.size(); ++ind) {
        codes[huffman_code[ind].first] = {huffman_code[ind].second, huffman_code_size[ind].second};
    }
    Print(bit_out, filename, in, is_end, huffman_code_size, codes);
}
