#include "unarchiver.h"

#include <cstdint>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include "bit_istream.h"

namespace {
const size_t FILENAME_END = 256;
const size_t ONE_MORE_FILE = 257;
const size_t ARCHIVE_END = 258;

const size_t DEFAULT_SIZE = 9;
}

std::vector<int16_t> GetSymbolOrder(BitIstream& bit_in, size_t symbols_count) {
    std::vector<int16_t> symbol_order;
    for (size_t ind = 0; ind < symbols_count; ++ind) {
        int16_t symbol;
        if (!bit_in.read(symbol, DEFAULT_SIZE)) {
            throw std::exception();
        }
        symbol_order.push_back(symbol);
    }
    return symbol_order;
}

void NextCode(int16_t& prefix, size_t& prefix_len, size_t& code_size, size_t count_code_size,
              std::vector<int16_t>& codes) {
    for (size_t ind = 0; ind < count_code_size; ++ind) {
        while (prefix_len < code_size) {
            prefix <<= 1;
            ++prefix_len;
        }
        codes.push_back(prefix);
        ++prefix;
        if (prefix >= (1 << (prefix_len + 1))) {
            ++prefix_len;
        }
    }
}

std::vector<int16_t> GetCodes(BitIstream& bit_in, size_t symbols_count) {
    std::vector<int16_t> codes;
    int16_t prefix = 1;
    size_t prefix_len = 0;
    size_t code_size = 1;
    while (codes.size() < symbols_count) {
        size_t count_code_size;
        if (!bit_in.read(count_code_size, DEFAULT_SIZE)) {
            throw std::exception();
        }
        NextCode(prefix, prefix_len, code_size, count_code_size,codes);
        ++code_size;
    }
    return codes;
}

bool WriteSymbolToFilename(int16_t code, std::vector<int16_t>& code_to_symbol, std::string& filename) {
    if (code_to_symbol[code] == FILENAME_END) {
        return true;
    }
    if (code_to_symbol[code] == ARCHIVE_END || code_to_symbol[code] == ONE_MORE_FILE) {
        throw std::exception();
    }
    filename.push_back(code_to_symbol[code]);
    return false;
}

std::string GetFilename(BitIstream& bit_in, std::vector<int16_t>& code_to_symbol) {
    std::string filename;
    int16_t code = 1;
    int16_t bit = 0;
    bool is_end_filename = false;
    while (bit_in.read(bit, 1)) {
        code <<= 1;
        code ^= bit;
        if (code >= code_to_symbol.size()) {
            throw std::exception();
        }
        if (code_to_symbol[code] != -1) {
            is_end_filename = WriteSymbolToFilename(code, code_to_symbol, filename);
            if (is_end_filename) {
                break;
            }
            code = 1;
        }
    }
    if (is_end_filename == false) {
        throw std::exception();
    }
    return filename;
}

int WriteSymbolToFile(std::ostream& out, int16_t code, std::vector<int16_t>& code_to_symbol) {
    if (code_to_symbol[code] == ONE_MORE_FILE) {
        out.flush();
        return ONE_MORE_FILE;
    }
    if (code_to_symbol[code] == ARCHIVE_END) {
        out.flush();
        return ARCHIVE_END;
    }
    if (code_to_symbol[code] == FILENAME_END) {
        throw std::exception();
    }
    char symbol = code_to_symbol[code];
    out.write(&symbol, sizeof(char));
    return 0;
}

bool PrintFile(BitIstream& bit_in, std::ostream& out, std::vector<int16_t>& code_to_symbol) {
    int16_t code = 1;
    int16_t bit = 0;
    while (bit_in.read(bit, 1)) {
        code <<= 1;
        code ^= bit;
        if (code >= code_to_symbol.size()) {
            throw std::exception();
        }
        if (code_to_symbol[code] != -1) {
            int write_type = WriteSymbolToFile(out, code, code_to_symbol);
            if (write_type == ONE_MORE_FILE) {
                return false;
            }
            if (write_type == ARCHIVE_END) {
                return true;
            }
            code = 1;
        }
    }
    throw std::exception();
}

const size_t MAX_CODE = (1 << 18);

void Unarchive(std::istream& in, std::vector<std::string>& files) {
    BitIstream bit_in(in);
    size_t symbols_count = 0;
    bool is_end = 0;
    while (!is_end && bit_in.read(symbols_count, DEFAULT_SIZE)) {
        auto symbol_order = GetSymbolOrder(bit_in, symbols_count);
        auto codes = GetCodes(bit_in, symbols_count);
        std::vector<int16_t> code_to_symbol(MAX_CODE, -1);
        for (size_t ind = 0; ind < symbols_count; ++ind) {
            code_to_symbol[codes[ind]] = symbol_order[ind];
        }
        auto filename = GetFilename(bit_in, code_to_symbol);
        files.push_back(filename);
        std::ofstream fout(filename);
        is_end = PrintFile(bit_in, fout, code_to_symbol);
    }
    if (is_end == false) {
        throw std::exception();
    }
}