#pragma once

#include "phrase_types.hpp"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vector>

int read_lzend_phrases(const char* filename, PhraseVector& phrases);
