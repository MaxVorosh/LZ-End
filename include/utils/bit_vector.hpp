#pragma once

#include "bit_writer.hpp"

#include <cstdint>
#include <cstring>
#include <utility>
#include <vector>

class SimpleBitVector {
public:
    SimpleBitVector(uint32_t n) : size_(0), capacity_(n), bytes_((n + 7) / 8) {
        if (n == 0) {
            capacity_ = 1;
            bytes_ = 1;
        }
        data_ = new uint8_t[bytes_];
        bw = BitWriter(data_);
    }

    SimpleBitVector(const SimpleBitVector&) = delete;
    SimpleBitVector& operator=(const SimpleBitVector&) = delete;

    SimpleBitVector(SimpleBitVector&& other) noexcept
        : size_(other.size_), capacity_(other.capacity_), bytes_(other.bytes_),
          data_(other.data_), bw(other.bw) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        other.bytes_ = 0;
        bw.update_ptr(data_);
    }

    SimpleBitVector& operator=(SimpleBitVector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            bytes_ = other.bytes_;
            data_ = other.data_;
            bw = other.bw;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
            other.bytes_ = 0;
            bw.update_ptr(data_);
        }
        return *this;
    }

    ~SimpleBitVector() {
        delete[] data_;
    }

    void write(int k, int bits) {
        if (size_ + bits >= capacity_) {
            capacity_ = capacity_ * 2;
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
    BitVector() : bv(1), data_(1) {}
    BitVector(SimpleBitVector&& bv);
    BitVector(const BitVector& other) = delete;
    BitVector(BitVector&& other) noexcept
        : bv(std::move(other.bv)), data_(std::move(other.data_)),
          superblocks(std::move(other.superblocks)),
          blocks(std::move(other.blocks)),
          block_size_(other.block_size_),
          superblock_size_(other.superblock_size_) {}

    BitVector& operator=(BitVector&& other) noexcept {
        if (this != &other) {
            bv = std::move(other.bv);
            data_ = std::move(other.data_);
            superblocks = std::move(other.superblocks);
            blocks = std::move(other.blocks);
            block_size_ = other.block_size_;
            superblock_size_ = other.superblock_size_;
        }
        return *this;
    }

    // Returns number of bits, equals to 'bit' on prefix i
    int rank(int i, int bit);
    // TODO
    // int select(int i, int bit);

private:
    SimpleBitVector bv;
    SimpleBitVector data_;
    std::vector<uint8_t> superblocks;
    std::vector<uint8_t> blocks;
    int block_size_;
    int superblock_size_;
};
