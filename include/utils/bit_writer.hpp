#pragma once

#include <cstdint>

class BitWriter {
public:
  BitWriter() : start_(nullptr), current_(nullptr) {}

  BitWriter(uint8_t* buffer)
    : start_(buffer),
      current_(buffer) {
  }

  // Write a single bit.
  void writeBit(int v);

  // Write a number of bits.
  void writeBits(int v, int count);

  int finish();

  void update_ptr(uint8_t* new_ptr) {
    current_ = new_ptr + (current_ - start_);
    start_ = new_ptr;
  }

private:
  // Write accumulated bits to buffer.
  void flush();

  uint8_t* start_;
  uint8_t* current_;
  uint32_t bits_ = 0;
  int position_ = 0;
  int bytes_written_ = 0;
};