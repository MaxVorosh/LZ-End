#pragma once

#include "utils/bit_writer.hpp"

#include <cstdint>
#include <cmath>

class HuffmanEncoder {
    struct Node {
        int freq;
        int symbol;
        Node* l;
        Node* r;
    };

    // For sorting on frequency.
    struct Comparator {
        bool operator()(const Node* l, const Node* r) {
            return l->freq > r->freq;
        }
    };

public:
    HuffmanEncoder(uint8_t* out) : writer_(out) {
        for (int i = 0; i < 512; ++i) {
            nodes_[i] = Node{0, i, 0, 0};
        }
    }

    void encode(char symbol);
    void scan(char symbol);
    void buildTable();
    void walk(Node* n, int level);
    void writeTable(int num_symbols);
    void buildCodes(int num_symbols);
    void limitLength(int num_symbols);
    
    int finish() {
        return writer_.finish();
    }
private:
    const int max_symbols_ = 256;
    const int kMaxHuffCodeLength = 11;

    uint8_t length_[256];
    int code_[256];
    Node nodes_[512];

    BitWriter writer_;
};
