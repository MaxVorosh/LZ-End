#include "utils.hpp"
#include <cstdlib>
#include <ctime>

namespace utils {

long double wclock() {
  return (long double)std::clock() / (long double)CLOCKS_PER_SEC;
}

void* allocate(const std::uint64_t size) {
  return std::malloc(size == 0 ? 1 : size);
}

void* aligned_allocate(const std::uint64_t size, const std::uint64_t) {
  return std::malloc(size == 0 ? 1 : size);
}

void deallocate(const void* const ptr) {
  std::free(const_cast<void*>(ptr));
}

void aligned_deallocate(const void* const ptr) {
  std::free(const_cast<void*>(ptr));
}

}  // namespace utils
