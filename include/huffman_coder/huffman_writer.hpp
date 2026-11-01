#pragma once

#include "base_coder/base_writer.hpp"

class HuffmanWriter : public BaseWriter {
    int write_lzend_phrases(const char* filename, const PhraseVector& phrases) override;
};