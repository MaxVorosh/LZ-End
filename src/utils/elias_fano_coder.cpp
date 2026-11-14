#include "utils/elias_fano_coder.hpp"

#include <cmath>

void EliasFanoCoder::build(PhraseVector& phrases, int n) {
    int z = phrases.size();
    int block_size = n / z;
    int blocks_cnt = (n + block_size - 1) / block_size;
    int cur_phrase = 0;
    int len_pref = 0;
    int max_len = block_size;

    int position_code_len = log2(block_size);

    SimpleBitVector positions_holder(1);
    SimpleBitVector lengths_holder(1);

    for (int i = 0; i < blocks_cnt; ++i) {
        int block_phrases_cnt = 0;
        while (cur_phrase < z && phrases[cur_phrase].m_len + 1 + len_pref < max_len) {
            max_len += phrases[cur_phrase].m_len + 1;
            positions_holder.write(block_phrases_cnt, position_code_len);
            block_phrases_cnt++;
            cur_phrase++;
        }
        lengths_holder.write((1 << (block_phrases_cnt + 1)) - 2, block_phrases_cnt + 1);
        max_len += blocks_cnt;
    }
}