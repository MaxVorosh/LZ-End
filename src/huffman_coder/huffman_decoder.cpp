#include "huffman_coder/huffman_decoder.hpp"

void HuffmanDecoder::readTable() {
  br_.refill();
  num_symbols_ = br_.readBits(sym_bits_);

  for (int i = 0; i < num_symbols_; ++i) {
    br_.refill();
    int symbol = br_.readBits(sym_bits_);
    int codelen = br_.readBits(4) + 1;

    ++codelen_count_[codelen];
    symbol_length_[i] = codelen;
    symbol_[i] = symbol;
    min_codelen_ = std::min(min_codelen_, codelen);
    max_codelen_ = std::max(max_codelen_, codelen);
  }

  // Ensure we catch up to be byte aligned.
  br_.byteAlign();
}

uint8_t HuffmanDecoder::decodeOne() {
  br_.refill();
  int n = br_.bits() >> (32 - max_codelen_);
  int len = bits_to_len_[n];
  br_.readBits(len);
  return bits_to_sym_[n];
}

void HuffmanDecoder::assignCodes() {
  int p = 0;
  uint8_t* cursym = &symbol_[0];
  for (int i = min_codelen_; i <= max_codelen_; ++i) {
    int n = codelen_count_[i];
    if (n) {
      int shift = max_codelen_ - i;
      memset(bits_to_len_ + p, i, n << shift);
      int m = 1 << shift;
      do {
        memset(bits_to_sym_ + p, *cursym++, m);
        p += m;
      } while(--n);
    }
  }
}