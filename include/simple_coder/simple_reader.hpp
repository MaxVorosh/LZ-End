#pragma once

#include "utils/phrase_types.hpp"
#include "base_coder/base_reader.hpp"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vector>

class SimpleReader : public BaseReader {
    int read_lzend_phrases(const char* filename, PhraseVector& phrases) override;
};
