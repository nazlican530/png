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

    // Çoklu thread ile negatif dönüşüm uygula
    applyNegativeThreaded(image);

    // Çıktıyı kaydet
    writePGM("output_threaded.pgm", image);

    std::cout << "Negatif (threaded) uygulandı ve 'output_threaded.pgm' olarak kaydedildi.\n";
    return 0;
}
