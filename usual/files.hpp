//
// Created by Theo Bessel on 2019-03-25.
//

#ifndef WEBRENDER_FILES_HPP
#define WEBRENDER_FILES_HPP

#include <fstream>
#include <vector>

namespace usual {
    static std::vector<char> readFile(const std::string& filename) {
        std::ifstream ifile(filename, std::ios::ate | std::ios::binary);

        if (!ifile.is_open()) {
            throw std::runtime_error("Failed to open file !");
        }

        size_t fileSize = (size_t) ifile.tellg();
        std::vector<char> buffer(fileSize);

        ifile.seekg(0);
        ifile.read(buffer.data(), fileSize);

        ifile.close();

        return buffer;
    }

    static std::string writeFile(std::string& text, const std::string& filename) {
        std::ofstream ofile;
        ofile.open(filename, std::ios::out | std::ios::app | std::ios::binary);
        if (!ofile.is_open()) {
            throw std::runtime_error("Failed to create file !");
        }
            ofile << text;
    }
}

#endif //WEBRENDER_FILES_HPP
