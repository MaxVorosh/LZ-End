#include "huffman_coder/huffman_writer.hpp"
#include "huffman_coder/huffman_encoder.hpp"

#include <fstream>

int HuffmanWriter::write_lzend_phrases(const char* filename, const PhraseVector& phrases) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        return -1;
    }
    uint64_t phrases_size = phrases.size();
    out.write(reinterpret_cast<const char*>(&phrases_size), sizeof(std::uint64_t));
    for (size_t i = 0; i < phrases.size(); ++i) {
        if (!out.write(reinterpret_cast<const char*>(&phrases[i].m_link), sizeof(std::uint64_t))) {
            return -1;
        }
        if (!out.write(reinterpret_cast<const char*>(&phrases[i].m_len), sizeof(std::uint64_t))) {
            return -1;
        }
    }
    int chunk_size = 256 * 1024;
    std::vector<uint8_t> buf(chunk_size * 2); // 256K chunk to encode
    for (size_t i = 0; i < phrases.size(); i+=chunk_size) {
        HuffmanEncoder encoder(buf.data());
        for (int j = 0; j < chunk_size; ++j) {
            if (i + j < phrases.size()) {
                encoder.scan(phrases[i + j].m_char);
            }
        }
        encoder.buildTable();
        for (int j = 0; j < chunk_size; ++j) {
            if (i + j < phrases.size()) {
                encoder.encode(phrases[i + j].m_char);
            }
        }
        int bytes_written = encoder.finish();
        if (!out.write(reinterpret_cast<const char*>(buf.data()), bytes_written)) {
            return -1;
        }
    }
    return out ? 0 : -1;
}
