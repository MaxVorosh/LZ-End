#include "initial_parser.hpp"
#include "lzend_writer.hpp"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::fprintf(stderr, "Usage: %s <compute> <input>\n", argv[0]);
    return 1;
  }
  const char* mode = argv[1];
  if (std::strcmp(mode, "write") != 0) {
    std::fprintf(stderr, "Only compute mode supported\n");
    return 1;
  }
  if (argc != 4) {
    std::fprintf(stderr, "Usage: %s compute <input>\n", argv[0]);
    return 1;
  }
  PhraseVector parsing;
  parse_to_lzend_phrases(argv[2], &parsing);
  write_lzend_phrases(argv[3], parsing);
  return 0;
}
