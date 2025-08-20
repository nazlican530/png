#include "pgm_reader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

PGMImage readPGM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Dosya açılamadı: " + filename);

    std::string magic;
    file >> magic;

    PGMType type;
    if (magic == "P2") type = PGMType::P2;
    else if (magic == "P5") type = PGMType::P5;
    else throw std::runtime_error("Desteklenmeyen format: " + magic);

    int width, height, maxVal;
    file >> width >> height >> maxVal;
    file.ignore(); // newline

    std::vector<unsigned char> data(width * height);

    if (type == PGMType::P2) {
        for (int i = 0; i < width * height; ++i) {
            int val;
            file >> val;
            data[i] = static_cast<unsigned char>(val);
        }
    } else {
        file.read(reinterpret_cast<char*>(data.data()), data.size());
    }

    return {width, height, maxVal, type, std::move(data)};
}

void writePGM(const std::string& filename, const PGMImage& image) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Dosya yazılamadı: " + filename);

    if (image.type == PGMType::P2)
        file << "P2\n";
    else
        file << "P5\n";

    file << image.width << " " << image.height << "\n";
    file << image.maxVal << "\n";

    if (image.type == PGMType::P2) {
        int count = 0;
        for (unsigned char pixel : image.pixels) {
            file << static_cast<int>(pixel) << " ";
            if (++count % image.width == 0) file << "\n";
        }
    } else {
        file.write(reinterpret_cast<const char*>(image.pixels.data()), image.pixels.size());
    }
}
