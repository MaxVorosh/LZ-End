#include "lzend_reader.hpp"

#include <fstream>

int read_lzend_phrases(const char* filename, PhraseVector& phrases) {
  std::ifstream in(filename, std::ios::binary);
  if (!in) {
    return -1;
  }
  while (true) {
    Phrase p{};
    in.read(reinterpret_cast<char*>(&p.m_link), sizeof(std::uint64_t));
    if (in.eof()) {
      break;
    }
    if (!in) {
      return -1;
    }
    in.read(reinterpret_cast<char*>(&p.m_len), sizeof(std::uint64_t));
    in.read(&p.m_char, sizeof(char));
    if (!in) {
      return -1;
    }
    phrases.push_back(p);
  }
  return 0;
}
