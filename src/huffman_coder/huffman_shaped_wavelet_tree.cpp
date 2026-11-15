#include "huffman_coder/huffman_shaped_wavelet_tree.hpp"
#include "huffman_coder/huffman_encoder.hpp"

void HuffmanShapedWaveletTree::build(PhraseVector& phrases) {
    int n = phrases.size();
    HuffmanEncoder encoder(nullptr);
    for (int i = 0; i < n; ++i) {
        encoder.scan(phrases[i].m_char);
    }
    encoder.buildTable();
    const uint64_t* codes = encoder.getCodes();
    const uint8_t* lengths = encoder.getLengths();
    std::vector<InBetweenWaveletNode> internal_nodes;
    internal_nodes.push_back(InBetweenWaveletNode{0, 0, SimpleBitVector(1), 0});
    for (int i = 0; i < n; ++i) {
        build_symb(phrases[i].m_char, codes[phrases[i].m_char], lengths[phrases[i].m_char], internal_nodes);
    }
    for (int i = 0; i < internal_nodes.size(); ++i) {
        nodes_.push_back(internal_nodes[i].convert());
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