#include <gtest/gtest.h>

#include "utils/elias_fano_coder.hpp"
#include "utils/phrase_types.hpp"

#include "compute_lzend.hpp"

#include <cstring>

// Build an LZ-End parsing using the external toolkit (compute_lzend),
// then convert the uint8_t phrase type into the project's PhraseVector.
static PhraseVector BuildParsing(const std::uint8_t* text, std::uint64_t len) {
  typedef lzend_phrase<std::uint8_t, std::uint64_t, std::uint64_t> U8Phrase;
  typedef space_efficient_vector<U8Phrase> U8Vector;

  U8Vector parsing;
  compute_lzend<std::uint8_t, std::uint64_t>(
      const_cast<std::uint8_t*>(text), len, &parsing);

  PhraseVector out;
  for (std::uint64_t i = 0; i < parsing.size(); ++i) {
    Phrase p;
    p.m_char = static_cast<char>(parsing[i].m_char);
    p.m_link = parsing[i].m_link;
    p.m_len = parsing[i].m_len;
    out.push_back(p);
  }
  return out;
}

TEST(EliasFanoTest, BuildConstantText) {
  std::uint64_t len = 100;
  std::uint8_t* text = new std::uint8_t[len];
  std::memset(text, 'a', len);
  PhraseVector parsing = BuildParsing(text, len);
  delete[] text;

  EliasFanoCoder coder;
  EXPECT_NO_THROW(coder.build(parsing, len));
}

TEST(EliasFanoTest, BuildTwoCharText) {
  std::uint64_t len = 50;
  std::uint8_t* text = new std::uint8_t[len];
  for (std::uint64_t i = 0; i < len; ++i) text[i] = (i % 2 == 0) ? 'a' : 'b';
  PhraseVector parsing = BuildParsing(text, len);
  delete[] text;

  EliasFanoCoder coder;
  EXPECT_NO_THROW(coder.build(parsing, len));
}

TEST(EliasFanoTest, BuildRepeatedPattern) {
  const char* pattern = "abcabc";
  std::uint64_t pat_len = std::strlen(pattern);
  std::uint64_t len = 1000;
  std::uint8_t* text = new std::uint8_t[len];
  for (std::uint64_t i = 0; i < len; ++i) text[i] = pattern[i % pat_len];
  PhraseVector parsing = BuildParsing(text, len);
  delete[] text;

  EliasFanoCoder coder;
  EXPECT_NO_THROW(coder.build(parsing, len));
}

TEST(EliasFanoTest, BuildSingleCharText) {
  std::uint64_t len = 1;
  std::uint8_t text[] = {'a'};
  PhraseVector parsing = BuildParsing(text, len);

  EliasFanoCoder coder;
  EXPECT_NO_THROW(coder.build(parsing, len));
}

TEST(EliasFanoTest, GettersReturnAfterBuild) {
  std::uint64_t len = 200;
  std::uint8_t* text = new std::uint8_t[len];
  std::memset(text, 'a', len);
  PhraseVector parsing = BuildParsing(text, len);
  delete[] text;

  EliasFanoCoder coder;
  coder.build(parsing, len);
  BitVector& lengths = coder.getLengths();
  BitVector& positions = coder.getPositions();
  (void)lengths;
  (void)positions;
}

TEST(EliasFanoTest, BuildRandomishText) {
  std::uint64_t len = 300;
  std::uint8_t* text = new std::uint8_t[len];
  for (std::uint64_t i = 0; i < len; ++i) {
    text[i] = 'a' + (i * 7 + 3) % 5;
  }
  PhraseVector parsing = BuildParsing(text, len);
  delete[] text;

  EliasFanoCoder coder;
  EXPECT_NO_THROW(coder.build(parsing, len));
}