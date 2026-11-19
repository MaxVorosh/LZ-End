#pragma once

#include "utils/bit_vector.hpp"
#include "utils/phrase_types.hpp"

#include <cstdint>
#include <iostream>

struct WaveletNode {
    uint32_t l = -1;
    uint32_t r = -1;
    BitVector index;
    char symb;
};

class HuffmanShapedWaveletTree {

    struct InBetweenWaveletNode {
        uint32_t l = -1;
        uint32_t r = -1;
        SimpleBitVector index;
        char symb;

        InBetweenWaveletNode() : index(1) {}
        InBetweenWaveletNode(uint32_t l_, uint32_t r_, SimpleBitVector&& idx, char s)
            : l(l_), r(r_), index(std::move(idx)), symb(s) {}

        InBetweenWaveletNode(const InBetweenWaveletNode&) = delete;
        InBetweenWaveletNode& operator=(const InBetweenWaveletNode&) = delete;

        InBetweenWaveletNode(InBetweenWaveletNode&& other) noexcept
            : l(other.l), r(other.r), index(std::move(other.index)), symb(other.symb) {
            other.l = -1;
            other.r = -1;
            other.symb = 0;
        }

        InBetweenWaveletNode& operator=(InBetweenWaveletNode&& other) noexcept {
            if (this != &other) {
                l = other.l; r = other.r; index = std::move(other.index); symb = other.symb;
                other.l = -1; other.r = -1; other.symb = 0;
            }
            return *this;
        }
    };

    WaveletNode convert(InBetweenWaveletNode&& node) {
        return WaveletNode{node.l, node.r, BitVector(std::move(node.index)), node.symb};
    }
public:
    void build(PhraseVector& phrases);

    std::vector<WaveletNode>& getTree() {
        return nodes_;
    }

    // Rank: count occurrences of symbol 'c' up to position 'pos' in the sequence
    uint32_t rank(char c, uint32_t pos) const;

private:
    void build_symb(char s, uint64_t code, uint8_t length, std::vector<InBetweenWaveletNode>& internal_nodes);
private:
    std::vector<WaveletNode> nodes_;
    uint64_t code_[256];
    uint8_t length_[256];
};