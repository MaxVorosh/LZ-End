#include "utils/elias_fano_coder.hpp"

#include <cmath>

void EliasFanoCoder::build() {
    int n = log2(numbers.back()) + 1;
    int m = log2(numbers.size());
    int prefix_len = m;
    int suffix_len = n - m;

    SimpleBitVector pref(numbers.size() * prefix_len);
    SimpleBitVector suff(numbers.size() * suffix_len);

    int last_prefix = 0;
    for (int i = 0; i < numbers.size(); ++i) {
        int prefix = numbers[i] >> suffix_len;
        int suffix = numbers[i] & ((1 << suffix_len) - 1);

        int prefix_diff = prefix - last_prefix;
        pref.write(0, prefix_diff);
        pref.write(1, 1);
        suff.write(suffix, suffix_len);
        last_prefix = prefix;
    }

    prefixes = BitVector(std::move(pref));
    suffixes = BitVector(std::move(suff));
}