#include "huffman_coder/huffman_reader.hpp"
#include "huffman_coder/huffman_decoder.hpp"

#include <fstream>
#include <vector>

int HuffmanReader::read_lzend_phrases(const char* filename, PhraseVector& phrases) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        return -1;
    }
    uint64_t number_of_symbols;
    in.read(reinterpret_cast<char*>(&number_of_symbols), sizeof(uint64_t));
    for (int i = 0; i < number_of_symbols; ++i) {
        phrases.push_back(Phrase{});
        in.read(reinterpret_cast<char*>(&phrases[i].m_link), sizeof(std::uint64_t));
        in.read(reinterpret_cast<char*>(&phrases[i].m_len), sizeof(std::uint64_t));
    }

    int phrase_pos = 0;
    std::vector<uint8_t> buf(512 * 1024); // 512K chunk to decode (256K decoded + spare 256K)
    while (!in.eof()) {
        in.read(reinterpret_cast<char*>(buf.data()), buf.size());
        HuffmanDecoder decoder(buf.data(), buf.data() + buf.size());
        decoder.readTable();
        decoder.assignCodes();
        for (int i = 0; i < decoder.length(); ++i) {
            phrases[phrase_pos].m_char = decoder.decodeOne();
            ++phrase_pos;
        }
    }
    return 0;
}