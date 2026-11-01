#include <gtest/gtest.h>

#include "base_coder/base_reader.hpp"
#include "base_coder/base_writer.hpp"
#include "huffman_coder/huffman_reader.hpp"
#include "huffman_coder/huffman_writer.hpp"
#include "simple_coder/simple_reader.hpp"
#include "simple_coder/simple_writer.hpp"
#include "utils/phrase_types.hpp"

#include <cstdio>
#include <vector>

static void RunRoundTrip(BaseWriter* w, BaseReader* r, const char* filename) {
  PhraseVector v;
  for (char c = 'a'; c <= 'c'; ++c) {
    Phrase p{};
    p.m_char = c;
    p.m_link = c - 'a';
    p.m_len = c - 'a' + 1;
    v.push_back(p);
  }
  ASSERT_EQ(w->write_lzend_phrases(filename, v), 0);
  PhraseVector read;
  ASSERT_EQ(r->read_lzend_phrases(filename, read), 0);
  ASSERT_EQ(read.size(), v.size());
  for (size_t i = 0; i < v.size(); ++i) {
    EXPECT_EQ(read[i].m_char, v[i].m_char);
    EXPECT_EQ(read[i].m_link, v[i].m_link);
    EXPECT_EQ(read[i].m_len, v[i].m_len);
  }
  std::remove(filename);
}

TEST(SimpleCoderTest, RoundTrip) {
  SimpleWriter w;
  SimpleReader r;
  RunRoundTrip(&w, &r, "simple_out.bin");
}

TEST(HuffmanCoderTest, RoundTrip) {
  HuffmanWriter w;
  HuffmanReader r;
  RunRoundTrip(&w, &r, "huff_out.bin");
}
