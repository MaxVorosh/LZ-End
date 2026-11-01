#include "huffman_coder/huffman_encoder.hpp"

#include <algorithm>

void HuffmanEncoder::encode(char symbol) {
  writer_.writeBits(code_[symbol], length_[symbol]);
}

void HuffmanEncoder::scan(char symbol) {
  ++nodes_[symbol].freq;
}

void HuffmanEncoder::buildTable() {
  // Coalesce used symbols, and add to heap
  Node* q[256];
  int num_symbols = 0;
  for (int i = 0; i < max_symbols_; ++i) {
    if (nodes_[i].freq) {
      nodes_[num_symbols] = nodes_[i];
      q[num_symbols] = &nodes_[num_symbols];
      ++num_symbols;
    }
  }
  Comparator c;
  std::make_heap(&q[0], &q[num_symbols], c);

  // Build Huffman tree
  for (int i = num_symbols; i > 1; --i) {
    Node* n1 = q[0];
    std::pop_heap(&q[0], &q[i], c);
    Node* n2 = q[0];
    std::pop_heap(&q[0], &q[i-1], c);

    Node* parent = &nodes_[num_symbols+i];
    parent->freq = n1->freq + n2->freq;
    parent->symbol = -1;
    parent->l = n2;
    parent->r = n1;
    q[i-2] = parent;
    std::push_heap(&q[0], &q[i-1], c);
  }
  // Label the distances from the root for the leafs
  walk(q[0], num_symbols == 1 ? 1 : 0);
  // Sort leaf nodes into level order.  This is required
  // for both length limiting and writing the table.
  std::sort(&nodes_[0], &nodes_[num_symbols],
            [](const Node& l, const Node& r){return l.freq < r.freq;});

  // limitLength(num_symbols);

  writeTable(num_symbols);
  buildCodes(num_symbols);
}

void HuffmanEncoder::walk(Node* n, int level) {
  if (n->symbol != -1) {
    // NOTE: re-using freq here to mean distance from root.
    n->freq = level;
    return;
  }

  walk(n->l, level + 1);
  walk(n->r, level + 1);
}

void HuffmanEncoder::writeTable(int num_symbols) {
  const int kSymBits = log2(max_symbols_);
  writer_.writeBits(num_symbols, kSymBits);

  for (int i = 0; i < num_symbols; ++i) {
    writer_.writeBits(nodes_[i].symbol, kSymBits);
    writer_.writeBits(nodes_[i].freq - 1, 4); // We are reusing frequesncies to store level
  }

  // Byte align after the table
  writer_.finish();
}

void HuffmanEncoder::buildCodes(int num_symbols) {
  int code = 0;
  int last_level = -1;
  for (int i = 0; i < num_symbols; ++i) {
    // Build the binary representation.
    int level = nodes_[i].freq;
    if (last_level != level) {
      if (last_level != -1) {
        ++code;
        code <<= (level - last_level);
      }
      last_level = level;
    } else {
      ++code;
    }

    int symbol = nodes_[i].symbol;
    length_[symbol] = level;
    code_[symbol] = code;
  }
}

void HuffmanEncoder::limitLength(int num_symbols) {
  // Limit the maximum code length
  int k = 0;
  int maxk = (1 << kMaxHuffCodeLength) - 1;
  for (int i = num_symbols - 1; i >= 0; --i) {
    nodes_[i].freq = std::min(nodes_[i].freq, kMaxHuffCodeLength);
    k += 1 << (kMaxHuffCodeLength - nodes_[i].freq);
  }
  for (int i = num_symbols - 1; i >= 0 && k > maxk; --i) {
    while (nodes_[i].freq < kMaxHuffCodeLength) {
      ++nodes_[i].freq;
      k -= 1 << (kMaxHuffCodeLength - nodes_[i].freq);
    } 
  }
  for (int i = 0; i < num_symbols; ++i) {
    while (k + (1 << (kMaxHuffCodeLength - nodes_[i].freq)) <= maxk) {
      k += 1 << (kMaxHuffCodeLength - nodes_[i].freq);
      --nodes_[i].freq;
    } 
  } 
} 