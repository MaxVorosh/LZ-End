#pragma once

#include "utils/phrase_types.hpp"

class BaseWriter {
public:
    virtual ~BaseWriter() = default;
    virtual int write_lzend_phrases(const char* filename, const PhraseVector& phrases) = 0;
};
