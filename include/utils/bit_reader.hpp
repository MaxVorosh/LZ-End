#pragma once

#include <cstdint>

class BitReader {
public:
  BitReader(uint8_t* buffer, uint8_t* end)
    : current_(buffer),
      end_(end) {
    refill();
  }

  // Consume one bit.  Result is 0 or 1.
  int readBit();

  int readBits(int count);

  void byteAlign();

  // Fill buffer with up to 32 bits.
  void refill();

  uint32_t bits() {
    return bits_;
  }

private:
  uint8_t* current_;
  uint8_t* end_;
  uint32_t bits_ = 0;
  int position_ = 24;
};