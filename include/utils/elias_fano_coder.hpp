#pragma once

#include "bit_vector.hpp"
#include "phrase_types.hpp"

class EliasFanoCoder {
public:
    void build(PhraseVector& phrases, int n);

    BitVector& getLengths() {
        return bucket_lengths;
    }

    BitVector& getPositions() {
        return positions;
    }
private:
    BitVector bucket_lengths;
    BitVector positions;
};