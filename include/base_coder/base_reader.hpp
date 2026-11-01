#pragma once

#include "utils/phrase_types.hpp"

class BaseReader {
public:
    virtual int read_lzend_phrases(const char* filename, PhraseVector& phrases) = 0;
};
