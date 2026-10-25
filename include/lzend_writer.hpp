#pragma once

#include "compute_lzend.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <fstream>

typedef lzend_phrase<char, std::uint64_t, std::uint64_t> Phrase;
typedef space_efficient_vector<Phrase> PhraseVector;

int write_lzend_phrases(const char* filename, const PhraseVector& phrases);

int write_symbol(std::ofstream* out, char symb);
int write_len(std::ofstream* out, std::uint64_t len);
int write_link(std::ofstream* out, std::uint64_t link);