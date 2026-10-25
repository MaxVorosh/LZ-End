#include "lzend_writer.hpp"
#include <cstdio>
#include <vector>

int main() {
  PhraseVector v;
  Phrase p{};
  p.m_char = 'a';
  p.m_link = 1;
  p.m_len = 2;
  v.push_back(p);
  if (write_lzend_phrases("test_out.bin", v) != 0) {
    std::fprintf(stderr, "write failed\n");
    return 1;
  }
  std::printf("OK\n");
  return 0;
}
