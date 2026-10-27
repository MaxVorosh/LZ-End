#pragma once

#include "phrase_types.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <fstream>

int write_lzend_phrases(const char* filename, const PhraseVector& phrases);

int write_symbol(std::ofstream* out, char symb);
int write_len(std::ofstream* out, std::uint64_t len);
int write_link(std::ofstream* out, std::uint64_t link);