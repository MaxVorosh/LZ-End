#pragma once

#include "compute_lzend.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

typedef lzend_phrase<char, std::uint64_t, std::uint64_t> Phrase;
typedef space_efficient_vector<Phrase> PhraseVector;
