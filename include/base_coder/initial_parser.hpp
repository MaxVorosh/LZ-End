#pragma once

#include "compute_lzend.hpp"

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

typedef lzend_phrase<char, std::uint64_t, std::uint64_t> Phrase;
typedef space_efficient_vector<Phrase> PhraseVector;

int parse_to_lzend_phrases(char* filename, PhraseVector* parsing) {
    std::ifstream in(filename, std::ios::binary | std::ios::ate);
    if (!in) {
        std::fprintf(stderr, "Failed to open file\n");
        return 1;
    }
    const std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);
    char* text = (size > 0) ? (char*)std::malloc((size_t)size) : (char*)std::malloc(1);
    if (size > 0) in.read(text, size);
    compute_lzend<char, std::uint64_t>(text, (std::uint64_t)size, parsing, false);
    std::printf("Number of phrases: %zu\n", parsing->size());
    std::free(text);
}