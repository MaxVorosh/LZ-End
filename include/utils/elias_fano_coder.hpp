#pragma once

#include "bit_vector.hpp"

#include <vector>

class EliasFanoCoder {
public:
    void build();

    void add_number(uint64_t number) {
        numbers.push_back(number);
    }

    BitVector& getPrefixes() {
        return prefixes;
    }

    BitVector& getSuffixes() {
        return suffixes;
    }
private:
    std::vector<uint64_t> numbers;

    BitVector prefixes;
    BitVector suffixes;
};