#include "simple_coder/simple_writer.hpp"

#include <fstream>

int SimpleWriter::write_lzend_phrases(const char* filename, const PhraseVector& phrases) {
  std::ofstream out(filename, std::ios::binary);
  if (!out) {
    return -1;
  }
  for (size_t i = 0; i < phrases.size(); ++i) {
    if (!out.write(reinterpret_cast<const char*>(&phrases[i].m_link), sizeof(std::uint64_t))) {
      return -1;
    }
    if (!out.write(reinterpret_cast<const char*>(&phrases[i].m_len), sizeof(std::uint64_t))) {
      return -1;
    }
    if (!out.write(&phrases[i].m_char, sizeof(char))) {
      return -1;
    }
  }
  return out ? 0 : -1;
}

