#include "huffman_coder/huffman_shaped_wavelet_tree.hpp"
#include "huffman_coder/huffman_encoder.hpp"

#include <algorithm>
#include <iostream>

void HuffmanShapedWaveletTree::build(PhraseVector& phrases) {
    int n = phrases.size();
    HuffmanEncoder encoder(nullptr);
    for (int i = 0; i < n; ++i) {
        encoder.scan(phrases[i].m_char);
    }
    encoder.buildTable();
    const uint64_t* codes = encoder.getCodes();
    const uint8_t* lengths = encoder.getLengths();

    for (int i = 0; i < 256; ++i) {
        code_[i] = codes[i];
        length_[i] = lengths[i];
    }
    
    std::vector<InBetweenWaveletNode> internal_nodes;
    internal_nodes.push_back(InBetweenWaveletNode{0, 0, SimpleBitVector(1), 0});
    for (int i = 0; i < n; ++i) {
        build_symb(phrases[i].m_char, codes[phrases[i].m_char], lengths[phrases[i].m_char], internal_nodes);
    }
    for (int i = 0; i < internal_nodes.size(); ++i) {
        nodes_.emplace_back(convert(std::move(internal_nodes[i])));
    }
}

void HuffmanShapedWaveletTree::build_symb(char s, uint64_t code, uint8_t length, std::vector<InBetweenWaveletNode>& internal_nodes) {
    int current_node = 0;
    for (int i = 0; i < length; ++i) {
        uint8_t bit = (code >> (length - i - 1)) % 2;
        if (internal_nodes[current_node].l == 0) {
            internal_nodes.push_back(InBetweenWaveletNode{0, 0, SimpleBitVector(1), 0});
            internal_nodes.push_back(InBetweenWaveletNode{0, 0, SimpleBitVector(1), 0});
            internal_nodes[current_node].l = internal_nodes.size() - 2;
            internal_nodes[current_node].r = internal_nodes.size() - 1;
        }
        int next_node = internal_nodes[current_node].l;
        if (bit == 1) {
            next_node = internal_nodes[current_node].r;
        }
        internal_nodes[current_node].index.write(bit, 1);
        current_node = next_node;
    }
    internal_nodes[current_node].symb = s;
}

uint32_t HuffmanShapedWaveletTree::rank(char c, uint32_t pos) const {
    uint8_t len = length_[static_cast<unsigned char>(c)];
    if (len == 0) return 0;
    
    uint64_t code = code_[static_cast<unsigned char>(c)];
    int current_node = 0;
    
    for (int i = 0; i < len; ++i) {
        uint8_t bit = (code >> (len - i - 1)) & 1;
        const WaveletNode& node = nodes_[current_node];
        uint32_t r = node.index.rank(pos, bit);
        if (r == 0) return 0;
        pos = r - 1;
        current_node = (bit == 0) ? node.l : node.r;
    }
    return 1;
}