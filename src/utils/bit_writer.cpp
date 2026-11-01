#include "utils/bit_writer.hpp"

#include <algorithm>

void BitWriter::writeBit(int v) {
  bits_ = (bits_ << 1) | v;
  ++position_;
  if (position_ >= 8) {
    flush();
  }
}

void BitWriter::writeBits(int v, int count) {
  while (count > 0) {
    int chunk = std::min(count, 8);
    bits_ = (bits_ << chunk) | v;
    position_ += chunk;
    count -= chunk;
    if (position_ >= 8) {
      flush();
    }
  }
}

void BitWriter::flush() {
  while (position_ >= 8) {
    position_ -= 8;
    *current_ = (bits_ >> position_) & 0xFF;
    ++current_;
    ++bytes_written_;
  }
}

int BitWriter::finish() {
  flush();
  if (position_ > 0) {
    int chunk = 8 - position_ % 8;
    writeBits(0, chunk);
  }
  return bytes_written_;
}

