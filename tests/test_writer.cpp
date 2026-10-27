#include <gtest/gtest.h>

#include "lzend_writer.hpp"

#include <cstdio>
#include <fstream>
#include <vector>

TEST(WriterTest, WritesSinglePhrase) {
  PhraseVector v;
  Phrase p{};
  p.m_char = 'a';
  p.m_link = 1;
  p.m_len = 2;
  v.push_back(p);

  const char* filename = "test_out.bin";
  ASSERT_EQ(write_lzend_phrases(filename, v), 0);

  std::ifstream in(filename, std::ios::binary);
  ASSERT_TRUE(in.is_open());

  std::uint64_t link, len;
  char symb;
  in.read(reinterpret_cast<char*>(&link), sizeof(link));
  in.read(reinterpret_cast<char*>(&len), sizeof(len));
  in.read(&symb, sizeof(symb));

  EXPECT_EQ(link, 1u);
  EXPECT_EQ(len, 2u);
  EXPECT_EQ(symb, 'a');

  in.close();
  std::remove(filename);
}

TEST(WriterTest, WritesMultiplePhrases) {
  PhraseVector v;
  for (char c = 'a'; c <= 'c'; ++c) {
    Phrase p{};
    p.m_char = c;
    p.m_link = c - 'a';
    p.m_len = c - 'a' + 1;
    v.push_back(p);
  }

  const char* filename = "test_out2.bin";
  ASSERT_EQ(write_lzend_phrases(filename, v), 0);

  std::ifstream in(filename, std::ios::binary);
  ASSERT_TRUE(in.is_open());

  for (char c = 'a'; c <= 'c'; ++c) {
    std::uint64_t link, len;
    char symb;
    in.read(reinterpret_cast<char*>(&link), sizeof(link));
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    in.read(&symb, sizeof(symb));
    EXPECT_EQ(link, static_cast<std::uint64_t>(c - 'a'));
    EXPECT_EQ(len, static_cast<std::uint64_t>(c - 'a' + 1));
    EXPECT_EQ(symb, c);
  }

  EXPECT_TRUE(in.eof() || in.good());
  in.close();
  std::remove(filename);
}

TEST(WriterTest, EmptyVector) {
  PhraseVector v;
  const char* filename = "test_empty.bin";
  ASSERT_EQ(write_lzend_phrases(filename, v), 0);

  std::ifstream in(filename, std::ios::binary);
  ASSERT_TRUE(in.is_open());
  in.seekg(0, std::ios::end);
  EXPECT_EQ(in.tellg(), 0);
  in.close();
  std::remove(filename);
}

TEST(WriterTest, InvalidFilenameReturnsError) {
  PhraseVector v;
  Phrase p{};
  p.m_char = 'x';
  p.m_link = 0;
  p.m_len = 1;
  v.push_back(p);
  EXPECT_EQ(write_lzend_phrases("/nonexistent_dir/out.bin", v), -1);
}
