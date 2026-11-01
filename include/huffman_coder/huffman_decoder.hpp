#pragma once

#include "utils/bit_reader.hpp"

#include <cstdint>
#include <cstring>
#include <algorithm>

class HuffmanDecoder {
public:
    HuffmanDecoder(uint8_t* in_start, uint8_t* in_end): br_(in_start, in_end) {
        std::fill(codelen_count_, codelen_count_ + 17, 0);
        std::fill(symbol_length_, symbol_length_ + 256, 0);
    }

    void readTable();
    uint8_t decodeOne();
    void assignCodes();

    int length() {
        return num_symbols_;
    }
private:
    const int sym_bits_ = 8;

    BitReader br_;

    int num_symbols_;
    int codelen_count_[17];
    int symbol_length_[256];
    uint8_t symbol_[256];
    int min_codelen_ = 256;
    int max_codelen_ = 0;
    char bits_to_len_[2048]; // 2^11, the max length of a symbol in decoder
    char bits_to_sym_[2048]; // 2^11, the max length of a symbol in decoder
};
