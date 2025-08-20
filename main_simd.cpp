#include "pgm_reader.h"
#include "image_operations.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Kullanım: " << argv[0] << " dosya.pgm\n";
        return 1;
    }

    // PGM dosyasını oku
    PGMImage image = readPGM(argv[1]);

    // SIMD ile negatif dönüşüm uygula
    applyNegativeSIMD(image);

    // Çıktıyı kaydet
    writePGM("output_simd.pgm", image);

    std::cout << "Negatif (SIMD) uygulandı ve 'output_simd.pgm' olarak kaydedildi.\n";
    return 0;
}
