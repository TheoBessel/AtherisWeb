//
// Created by Theo Bessel on 2019-03-25.
//

#ifndef WEBRENDER_FILES_HPP
#define WEBRENDER_FILES_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace usual {
    static std::string readFile(fs::path path) {
        std::ifstream ifile{ path };

        if (!ifile.is_open()) {
            throw std::runtime_error("Failed to open file !");
        }

        const auto fileSize = fs::file_size(path);

        std::string result(fileSize, ' ');

        ifile.read(result.data(), fileSize);
        return result;
    }

    static std::vector<char> readFileToBuffer(const std::string &filename) {
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
    };

    static void writeFile(const std::string& text, const std::string& filename) {
        std::ofstream ofile;
        ofile.open(filename, std::ios::out | std::ios::app | std::ios::binary);
        if (!ofile.is_open()) {
            throw std::runtime_error("Failed to create file !");
        }
        ofile << text;

        ofile.close();
    }
}

#endif //WEBRENDER_FILES_HPP
