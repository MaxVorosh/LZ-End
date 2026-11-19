#include <gtest/gtest.h>

#include "huffman_coder/huffman_shaped_wavelet_tree.hpp"
#include "utils/phrase_types.hpp"

#include <cstdint>
#include <vector>

static PhraseVector MakePhrases(const std::vector<std::pair<char, std::uint64_t>>& specs) {
  PhraseVector phrases;
  for (auto& s : specs) {
    Phrase p{};
    p.m_char = s.first;
    p.m_link = s.second;
    p.m_len = s.second + 1;
    phrases.push_back(p);
  }
  return phrases;
}

TEST(HuffmanShapedWaveletTreeTest, BuildSingleSymbol) {
  auto phrases = MakePhrases({{'a', 0}, {'a', 1}, {'a', 2}});
  HuffmanShapedWaveletTree tree;
  EXPECT_NO_THROW(tree.build(phrases));
  auto& nodes = tree.getTree();
  EXPECT_GE(nodes.size(), 1u);
}

TEST(HuffmanShapedWaveletTreeTest, BuildTwoSymbols) {
  auto phrases = MakePhrases({{'a', 0}, {'b', 1}, {'a', 2}, {'b', 3}});
  HuffmanShapedWaveletTree tree;
  EXPECT_NO_THROW(tree.build(phrases));
  auto& nodes = tree.getTree();
  EXPECT_GE(nodes.size(), 1u);
}

TEST(HuffmanShapedWaveletTreeTest, BuildManySymbols) {
  std::vector<std::pair<char, std::uint64_t>> specs;
  for (int i = 0; i < 10; ++i) {
    specs.push_back({static_cast<char>('a' + i), static_cast<std::uint64_t>(i)});
  }
  auto phrases = MakePhrases(specs);
  HuffmanShapedWaveletTree tree;
  EXPECT_NO_THROW(tree.build(phrases));
  auto& nodes = tree.getTree();
  EXPECT_GE(nodes.size(), 1u);
}

TEST(HuffmanShapedWaveletTreeTest, RootNodeHasChildren) {
  auto phrases = MakePhrases({{'a', 0}, {'b', 1}, {'c', 2}});
  HuffmanShapedWaveletTree tree;
  tree.build(phrases);
  auto& nodes = tree.getTree();
  EXPECT_GE(nodes.size(), 1u);
  EXPECT_NE(nodes[0].l, static_cast<uint32_t>(-1));
  EXPECT_NE(nodes[0].r, static_cast<uint32_t>(-1));
}

TEST(HuffmanShapedWaveletTreeTest, SymbolsStoredAtLeaves) {
  auto phrases = MakePhrases({{'a', 0}, {'b', 1}, {'c', 2}});
  HuffmanShapedWaveletTree tree;
  tree.build(phrases);
  auto& nodes = tree.getTree();
  int symb_count = 0;
  for (auto& n : nodes) {
    if (n.symb != 0) ++symb_count;
  }
  EXPECT_EQ(symb_count, 3);
}

TEST(HuffmanShapedWaveletTreeTest, BuildRepeatedPhrases) {
  std::vector<std::pair<char, std::uint64_t>> specs;
  for (int i = 0; i < 100; ++i) {
    specs.push_back({static_cast<char>('a' + (i % 5)), static_cast<std::uint64_t>(i)});
  }
  auto phrases = MakePhrases(specs);
  HuffmanShapedWaveletTree tree;
  EXPECT_NO_THROW(tree.build(phrases));
  auto& nodes = tree.getTree();
  EXPECT_GE(nodes.size(), 1u);
}

TEST(HuffmanShapedWaveletTreeTest, GettersReturnReference) {
  auto phrases = MakePhrases({{'a', 0}, {'b', 1}});
  HuffmanShapedWaveletTree tree;
  tree.build(phrases);
  std::vector<WaveletNode>& nodes = tree.getTree();
  EXPECT_GE(nodes.size(), 1u);
}

TEST(HuffmanShapedWaveletTreeTest, RankSingleSymbol) {
  auto phrases = MakePhrases({{'a', 0}, {'a', 1}, {'a', 2}, {'a', 3}, {'a', 4}});
  HuffmanShapedWaveletTree tree;
  tree.build(phrases);
  EXPECT_EQ(tree.rank('a', 0), 1u);
  EXPECT_EQ(tree.rank('a', 1), 2u);
  EXPECT_EQ(tree.rank('a', 2), 3u);
  EXPECT_EQ(tree.rank('a', 4), 5u);
  EXPECT_EQ(tree.rank('a', 10), 5u);
}

TEST(HuffmanShapedWaveletTreeTest, RankTwoSymbols) {
  auto phrases = MakePhrases({{'a', 0}, {'b', 1}, {'a', 2}, {'b', 3}, {'a', 4}});
  HuffmanShapedWaveletTree tree;
  tree.build(phrases);
  EXPECT_EQ(tree.rank('a', 0), 1u);
  EXPECT_EQ(tree.rank('a', 1), 1u);
  EXPECT_EQ(tree.rank('a', 2), 2u);
  EXPECT_EQ(tree.rank('a', 3), 2u);
  EXPECT_EQ(tree.rank('a', 4), 3u);
  EXPECT_EQ(tree.rank('b', 0), 0u);
  EXPECT_EQ(tree.rank('b', 1), 1u);
  EXPECT_EQ(tree.rank('b', 2), 1u);
  EXPECT_EQ(tree.rank('b', 3), 2u);
  EXPECT_EQ(tree.rank('b', 4), 2u);
}

TEST(HuffmanShapedWaveletTreeTest, RankThreeSymbols) {
  auto phrases = MakePhrases({{'a', 0}, {'b', 1}, {'c', 2}, {'a', 3}, {'b', 4}, {'c', 5}});
  HuffmanShapedWaveletTree tree;
  tree.build(phrases);
  EXPECT_EQ(tree.rank('a', 0), 1u);
  EXPECT_EQ(tree.rank('a', 1), 1u);
  EXPECT_EQ(tree.rank('a', 2), 1u);
  EXPECT_EQ(tree.rank('a', 3), 2u);
  EXPECT_EQ(tree.rank('b', 0), 0u);
  EXPECT_EQ(tree.rank('b', 1), 1u);
  EXPECT_EQ(tree.rank('b', 2), 1u);
  EXPECT_EQ(tree.rank('b', 3), 1u);
  EXPECT_EQ(tree.rank('b', 4), 2u);
  EXPECT_EQ(tree.rank('c', 0), 0u);
  EXPECT_EQ(tree.rank('c', 1), 0u);
  EXPECT_EQ(tree.rank('c', 2), 1u);
  EXPECT_EQ(tree.rank('c', 3), 1u);
  EXPECT_EQ(tree.rank('c', 4), 1u);
  EXPECT_EQ(tree.rank('c', 5), 2u);
}

TEST(HuffmanShapedWaveletTreeTest, RankUnknownSymbol) {
  auto phrases = MakePhrases({{'a', 0}, {'b', 1}});
  HuffmanShapedWaveletTree tree;
  tree.build(phrases);
  EXPECT_EQ(tree.rank('c', 0), 0u);
  EXPECT_EQ(tree.rank('z', 10), 0u);
}

TEST(HuffmanShapedWaveletTreeTest, RankPositionOutOfBounds) {
  auto phrases = MakePhrases({{'a', 0}, {'a', 1}, {'a', 2}});
  HuffmanShapedWaveletTree tree;
  tree.build(phrases);
  EXPECT_EQ(tree.rank('a', 2), 3u);
  EXPECT_EQ(tree.rank('a', 100), 3u);
}