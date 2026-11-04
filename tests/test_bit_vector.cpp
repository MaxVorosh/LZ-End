#include <gtest/gtest.h>

#include "utils/bit_vector.hpp"
#include <iostream>

TEST(BitVectorTest, RankSelect) {
  SimpleBitVector sbv(64);
  for (int i = 0; i < 64; i++) { 
    sbv.write(1, 1);
    sbv.write(0, 1);
  };
  sbv.finish();
  BitVector bv(sbv);
  EXPECT_EQ(bv.rank(4, 1), 2);
  EXPECT_EQ(bv.rank(4, 0), 2);
  // EXPECT_EQ(bv.select(2, 1), 2);
}
