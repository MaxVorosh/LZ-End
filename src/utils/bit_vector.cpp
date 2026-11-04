#include "utils/bit_vector.hpp"

#include <cmath>
#include <iostream>

BitVector::BitVector(SimpleBitVector&& data) : bv(data.getSize()), data_(std::move(data)) {
    int size = data_.getSize();
    
    // Number of elements
    bv.write(size, 64); 
    if (size == 0) {
        return;
    }

    int log = log2(size);
    block_size_ = log / 2;
    superblock_size_ = log * log / 2;
    int block_value_size = log2(block_size_);
    int superblock_value_size = log2(superblock_size_);

    // Superblock ranks
    int byte_cnt = 0;
    int current_sb_pos = 0;
    int current_sb_count = 0;
    superblocks.reserve((size + superblock_size_ - 1) / superblock_size_);
    superblocks.push_back(0);
    for (int i = 0; i < size; i += 8) {
        for (int j = 0; j < 8; ++j) {
            if (i + j > size) {
                break;
            }
            int bit = (data_.get_byte(byte_cnt) >> (7 - j)) % 2;
            current_sb_count += bit;
            current_sb_pos += 1;
            if (current_sb_pos == superblock_size_) {
                bv.write(current_sb_count, superblock_value_size);
                superblocks.push_back(current_sb_count);
                current_sb_count = 0;
                current_sb_pos = 0;
            }
        }
        byte_cnt++;
    }

    // Macroblocks ranks
    byte_cnt = 0;
    int current_mb_pos = 0;
    int current_mb_count = 0;
    blocks.reserve((size + block_size_ - 1) / block_size_);
    blocks.push_back(0);
    for (int i = 0; i < size; i += 8) {
        for (int j = 0; j < 8; ++j) {
            if (i + j > size) {
                if (current_mb_pos != 0) {
                    bv.write(current_mb_count, block_value_size);
                }
                break;
            }
            int bit = (data_.get_byte(byte_cnt) >> (7 - j)) % 2;
            current_mb_count += bit;
            current_mb_pos += 1;
            if (current_mb_pos == block_size_) {
                bv.write(current_mb_count, block_value_size);
                blocks.push_back(current_mb_count);
                current_mb_count = 0;
                current_mb_pos = 0;
            }
        }
        byte_cnt++;
    }

    // Chunk table
    uint64_t max_block_values = 1 << (block_size_);
    table.resize(max_block_values);
    for (uint64_t i = 0; i < max_block_values; ++i) {
        int rank = __builtin_popcount(i);
        bv.write(rank, block_value_size);
        table.push_back(i);
    }

    // Original data
    byte_cnt = 0;
    for (int i = 0; i < size; i += 8) {
        int chunk = (i + 8 > size) ? size - i : 8;
        bv.write(data_.get_byte(byte_cnt), chunk);
        byte_cnt++;
    }
    bv.finish();
}

int BitVector::rank(int i, int bit) {
    int byte = i / 8;
    int in_byte_shift = i % 8;
    int sb = i / superblock_size_;
    int mb = i / block_size_;
    int mask = (1 << block_size_) - 1;
    int double_byte = (data_.get_byte(byte) << 8) + data_.get_byte(byte + 1);
    int shift = 16 - in_byte_shift - block_size_;
    int chunk = (double_byte >> shift) & mask;
    int ans = superblocks[sb] + blocks[mb] + table[chunk];
    if (bit == 0) {
        return i - ans;
    }
    return ans;
}