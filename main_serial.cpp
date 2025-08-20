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

    // Sıradan negatif dönüşüm uygula
    applyNegative(image);

    // Çıktıyı kaydet
    writePGM("output_serial.pgm", image);

    std::cout << "Negatif (sıradan) uygulandı ve 'output_serial.pgm' olarak kaydedildi.\n";
    return 0;
}
