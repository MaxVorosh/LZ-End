#include "utils/bit_reader.hpp"

int BitReader::readBit() {
  int r = bits_ >> 31;
  bits_ <<= 1;
  ++position_;
  if (position_ == 24) {
    refill();
  }
  return r;
}

int BitReader::readBits(int count) {
  if (count + position_ <= 24) {
    int r = bits_ >> (32 - count);
    bits_ <<= count;
    position_ += count;
    return r;  
  }
  int first_chunk = count + position_ - 24;
  int res1 = readBits(first_chunk);
  refill();
  int res2 = readBits(count - first_chunk);
  return (res1 << (count - first_chunk)) + res2;
}

void BitReader::byteAlign() {
  int chunk = (8 - position_ % 8) % 8;
  readBits(chunk);
}

void BitReader::refill() {
  while (position_ >= 0) {
    bits_ |= (current_ < end_ ? *current_ : 0) << position_;
    position_ -= 8;
    ++current_;
  }
}
