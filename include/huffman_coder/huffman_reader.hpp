#pragma once

#include "base_coder/base_reader.hpp"

class HuffmanReader : public BaseReader {
    int read_lzend_phrases(const char* filename, PhraseVector& phrases) override;
};