#include "pgm_reader.h"
#include <immintrin.h>

void applyNegativeSIMD(PGMImage& image) {
    auto& data = image.pixels;
    size_t size = data.size();
    size_t i = 0;

    for (; i + 31 < size; i += 32) {
        __m256i pixels = _mm256_loadu_si256((__m256i*)&data[i]);
        __m256i max_val = _mm256_set1_epi8((char)255);
        __m256i result = _mm256_sub_epi8(max_val, pixels);
        _mm256_storeu_si256((__m256i*)&data[i], result);
    }

    for (; i < size; ++i) {
        data[i] = 255 - data[i];
    }
}
