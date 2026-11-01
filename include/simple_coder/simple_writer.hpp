#pragma once

#include "utils/phrase_types.hpp"
#include "base_coder/base_writer.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <fstream>

class SimpleWriter : public BaseWriter {
    int write_lzend_phrases(const char* filename, const PhraseVector& phrases) override;
};
