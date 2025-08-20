#include "image_operations.h"
#include <omp.h>
#include <vector>
#include <thread>
#include <immintrin.h>

// Histogram hesaplama (OpenMP ile paralel)
std::vector<int> computeHistogram(const PGMImage& image) {
    std::vector<int> histogram(256, 0);
    int num_threads = omp_get_max_threads();
    std::vector<std::vector<int>> local_histograms(num_threads, std::vector<int>(256, 0));

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        #pragma omp for
        for (int i = 0; i < static_cast<int>(image.pixels.size()); ++i) {
            unsigned char pixel = image.pixels[i];
            local_histograms[tid][pixel]++;
        }
    }

    for (int t = 0; t < num_threads; ++t) {
        for (int i = 0; i < 256; ++i) {
            histogram[i] += local_histograms[t][i];
        }
    }

    return histogram;
}

// CDF hesaplama
std::vector<int> computeCDF(const std::vector<int>& histogram) {
    std::vector<int> cdf(256, 0);
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; ++i) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }
    return cdf;
}

// Sıradan negatif görüntü
void applyNegative(PGMImage& image) {
    for (auto& pixel : image.pixels) {
        pixel = 255 - pixel;
    }
}

// Histogram eşitleme
void applyHistogramEqualization(PGMImage& image) {
    auto histogram = computeHistogram(image);
    auto cdf = computeCDF(histogram);
    int total_pixels = image.pixels.size();
    std::vector<unsigned char> equalized_lut(256, 0);

    for (int i = 0; i < 256; ++i) {
        equalized_lut[i] = static_cast<unsigned char>((cdf[i] - cdf[0]) * 255 / (total_pixels - 1));
    }

    for (auto& pixel : image.pixels) {
        pixel = equalized_lut[pixel];
    }
}

// Thread ile negatif görüntü
void negativeThreadWorker(unsigned char* data, int start, int end) {
    for (int i = start; i < end; ++i) {
        data[i] = 255 - data[i];
    }
}

void applyNegativeThreaded(PGMImage& image) {
    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    int chunk_size = image.pixels.size() / num_threads;

    for (int t = 0; t < num_threads; ++t) {
        int start = t * chunk_size;
        int end = (t == num_threads - 1) ? image.pixels.size() : (t + 1) * chunk_size;
        threads.emplace_back(negativeThreadWorker, image.pixels.data(), start, end);
    }

    for (auto& th : threads) {
        th.join();
    }
}
