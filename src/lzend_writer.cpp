#include "lzend_writer.hpp"

#include <fstream>

int write_lzend_phrases(const char* filename, const PhraseVector& phrases) {
  std::ofstream out(filename, std::ios::binary);
  if (!out) {
    return -1;
  }
  for (size_t i = 0; i < phrases.size(); ++i) {
    int err;
    if ((err = write_link(&out, phrases[i].m_link))) {
      return err;
    }
    if ((err = write_len(&out, phrases[i].m_len))) {
      return err;
    }
    if ((err = write_symbol(&out, phrases[i].m_char))) {
      return err;
    }
  }
  return out ? 0 : -1;
}

int write_symbol(std::ofstream* out, char symb) {
    out->write(&symb, sizeof(char));
    return out ? 0 : -1;
}

int write_len(std::ofstream* out, std::uint64_t len) {
    out->write(reinterpret_cast<const char*>(&len), sizeof(std::uint64_t));
    return out ? 0 : -1;
}

int write_link(std::ofstream* out, std::uint64_t link) {
    out->write(reinterpret_cast<const char*>(&link), sizeof(std::uint64_t));
    return out ? 0 : -1;
}
