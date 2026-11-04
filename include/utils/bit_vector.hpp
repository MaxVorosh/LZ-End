#pragma once

#include "bit_writer.hpp"

#include <cstdint>
#include <cstring>
#include <vector>

class SimpleBitVector {
public:
    SimpleBitVector(uint32_t n) : size_(0), capacity_(n), bytes_((n + 7) / 8) {
        data_ = new uint8_t[bytes_];
        bw = BitWriter(data_);
    }

    ~SimpleBitVector() {
        delete[] data_;
    }

    void write(int k, int bits) {
        if (size_ + bits >= capacity_) {
            capacity_ *= 2;
            int old_bytes = bytes_;
            bytes_ = (capacity_ + 7) / 8;
            uint8_t* old_data = data_;
            data_ = new uint8_t[bytes_];
            memcpy(data_, old_data, old_bytes);
            bw.update_ptr(data_);
            delete[] old_data;
        }
        bw.writeBits(k, bits);
        size_ += bits;
    }

    void finish() {
        bw.finish();
    }

    uint32_t getSize() const {
        return size_;
    }

    uint8_t get_byte(int i) const {
        if (i * 8 > size_) {
            return 0;
        }
        return data_[i];
    }
private:
    uint32_t size_; // in bits
    uint32_t capacity_; // in bits
    uint32_t bytes_;
    uint8_t* data_;
    BitWriter bw;
};

class BitVector {
public:
    BitVector() : bv(1) {}
    BitVector(const SimpleBitVector& bv);

    // Returns number of bits, equals to 'bit' on prefix i
    int rank(int i, int bit);
    // TODO
    // int select(int i, int bit);

private:
    SimpleBitVector bv;
    const SimpleBitVector* data_;
    std::vector<uint8_t> superblocks;
    std::vector<uint8_t> blocks;
    std::vector<uint8_t> table;
    int block_size_;
    int superblock_size_;
};
