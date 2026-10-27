#include <gtest/gtest.h>

#include "lzend_reader.hpp"
#include "lzend_writer.hpp"

#include <cstdio>
#include <fstream>
#include <vector>

TEST(WriterReaderTest, RoundTripSingle) {
  PhraseVector v;
  Phrase p{};
  p.m_char = 'a';
  p.m_link = 1;
  p.m_len = 2;
  v.push_back(p);

  const char* filename = "test_out.bin";
  ASSERT_EQ(write_lzend_phrases(filename, v), 0);

  PhraseVector read;
  ASSERT_EQ(read_lzend_phrases(filename, read), 0);
  ASSERT_EQ(read.size(), v.size());
  EXPECT_EQ(read[0].m_char, v[0].m_char);
  EXPECT_EQ(read[0].m_link, v[0].m_link);
  EXPECT_EQ(read[0].m_len, v[0].m_len);

  std::remove(filename);
}

TEST(WriterReaderTest, RoundTripMultiple) {
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

  PhraseVector read;
  ASSERT_EQ(read_lzend_phrases(filename, read), 0);
  ASSERT_EQ(read.size(), v.size());
  for (size_t i = 0; i < v.size(); ++i) {
    EXPECT_EQ(read[i].m_char, v[i].m_char);
    EXPECT_EQ(read[i].m_link, v[i].m_link);
    EXPECT_EQ(read[i].m_len, v[i].m_len);
  }

  std::remove(filename);
}

TEST(WriterReaderTest, EmptyVector) {
  PhraseVector v;
  const char* filename = "test_empty.bin";
  ASSERT_EQ(write_lzend_phrases(filename, v), 0);

  PhraseVector read;
  ASSERT_EQ(read_lzend_phrases(filename, read), 0);
  EXPECT_EQ(read.size(), 0u);

  std::remove(filename);
}

TEST(WriterReaderTest, InvalidFilenameReturnsError) {
  PhraseVector v;
  Phrase p{};
  p.m_char = 'x';
  p.m_link = 0;
  p.m_len = 1;
  v.push_back(p);
  EXPECT_EQ(write_lzend_phrases("/nonexistent_dir/out.bin", v), -1);

  PhraseVector read;
  EXPECT_EQ(read_lzend_phrases("/nonexistent_dir/out.bin", read), -1);
}
