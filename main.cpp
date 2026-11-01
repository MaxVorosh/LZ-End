#include "base_coder/base_writer.hpp"
#include "base_coder/initial_parser.hpp"
#include "simple_coder/simple_writer.hpp"
#include "huffman_coder/huffman_writer.hpp"

#include <cstdio>
#include <cstring>

int main(int argc, char** argv) {
  if (argc < 4) {
    std::fprintf(stderr, "Usage: %s write <input> <output> [huffman]\n", argv[0]);
    return 1;
  }
  bool huffman = (argc >= 5 && std::strcmp(argv[4], "huffman") == 0);
  PhraseVector parsing;
  parse_to_lzend_phrases(const_cast<char*>(argv[2]), &parsing);
  BaseWriter* writer = huffman ? static_cast<BaseWriter*>(new HuffmanWriter())
                               : static_cast<BaseWriter*>(new SimpleWriter());
  int rc = writer->write_lzend_phrases(argv[3], parsing);
  delete writer;
  if (rc != 0) {
    std::fprintf(stderr, "write failed\n");
    return 1;
  }
  return 0;
}
