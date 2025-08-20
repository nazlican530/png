#include "pgm_reader.h"
#include "image_operations.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Kullanım: " << argv[0] << " dosya.pgm\n";
        return 1;
    }

    try {
        PGMImage image = readPGM(argv[1]);

        std::cout << "Genişlik: " << image.width << "\n";
        std::cout << "Yükseklik: " << image.height << "\n";
        std::cout << "Maks. Piksel: " << image.maxVal << "\n";
        std::cout << "Format: " << (image.type == PGMType::P2 ? "ASCII (P2)" : "Binary (P5)") << "\n";

        auto histogram = computeHistogram(image);
        auto cdf = computeCDF(histogram);

        std::cout << "\nHistogram (ilk 10 değer): ";
        for (int i = 0; i < 10; ++i)
            std::cout << histogram[i] << " ";

        std::cout << "\nCDF (ilk 10 değer): ";
        for (int i = 0; i < 10; ++i)
            std::cout << cdf[i] << " ";
        std::cout << "\n";

        applyNegative(image);

        // Gprof için yapay işlem bloğu
        volatile int dummy = 0;
        for (int i = 0; i < 500000000; ++i) {
            dummy += i;
        }
        std::cout << "Dummy sonucu: " << dummy << std::endl;

        applyHistogramEqualization(image);
        std::cout << "Histogram eşitleme uygulandı.\n";

        writePGM("output.pgm", image);
        std::cout << "İşlenmiş görüntü 'output.pgm' olarak kaydedildi.\n";

    } catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
