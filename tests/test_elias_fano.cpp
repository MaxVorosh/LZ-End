#include <gtest/gtest.h>

#include "utils/elias_fano_coder.hpp"

#include <vector>

TEST(EliasFanoTest, BuildSequential) {
  EliasFanoCoder coder;
  for (uint64_t i = 0; i < 16; ++i) {
    coder.add_number(i * 3);
  }
  EXPECT_NO_THROW(coder.build());
}

TEST(EliasFanoTest, BuildPowersOfTwo) {
  EliasFanoCoder coder;
  for (int i = 0; i < 8; ++i) {
    coder.add_number(1ULL << i);
  }
  EXPECT_NO_THROW(coder.build());
}

TEST(EliasFanoTest, BuildLargeGap) {
  EliasFanoCoder coder;
  coder.add_number(0);
  coder.add_number(1000);
  coder.add_number(2000);
  EXPECT_NO_THROW(coder.build());
}

TEST(EliasFanoTest, ReferencesValidAfterBuild) {
  EliasFanoCoder coder;
  for (uint64_t i = 0; i < 8; ++i) {
    coder.add_number(i);
  }
  coder.build();
  BitVector& prefixes = coder.getPrefixes();
  BitVector& suffixes = coder.getSuffixes();
  (void)prefixes;
  (void)suffixes;
}
