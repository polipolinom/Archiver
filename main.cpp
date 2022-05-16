#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "archiver_lib/archiver.h"
#include "archiver_lib/unarchiver.h"

void Compress(std::ofstream& out, std::vector<char*> files) {
    for (size_t ind = 0; ind < files.size(); ++ind) {
        auto file = files[ind];
        std::ifstream in(file, std::ios::binary);
        if (!in.good()) {
            std::remove(file);
            std::cout << "Error: can't find or open file " << file << std::endl;
            return;
        }
        bool is_end = (ind + 1 == files.size());
        Archive(in, out, file, is_end);
    }
}

void Compressive(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "You need to write the name of the archived file";
        return;
    }
    std::ofstream out(argv[2], std::ios::binary);
    std::vector<char*> files;
    for (size_t ind = 3; ind < argc; ++ind) {
        files.push_back(argv[ind]);
    }
    Compress(out, files);
}

void Decompressive(int argc, char* argv[]) {
    std::ifstream fin(argv[2], std::ios::binary);
    std::vector<std::string> files;
    if (!fin.good()) {
        std::cout << "Error: can't find or open file " << argv[2] << std::endl;
        return;
    }
    try {
        Unarchive(fin, files);
    } catch (std::exception& e) {
        for (const auto& filename : files) {
            std::remove(filename.c_str());
        }
        std::cout << "Error: file for unarchive is not correct" << std::endl;
        return;
    }
}

void Help() {
    std::cout << "For archive files print -d archive_name file1 file2 ...." << std::endl;
    std::cout << "For unarchive files print -c archive_name" << std::endl;
    std::cout << "For help print -h" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        Help();
        return 0;
    }
    if (!strcmp(argv[1], "-c")) {
        Compressive(argc, argv);
        return 0;
    }
    if (!strcmp(argv[1], "-d")) {
        Decompressive(argc, argv);
        return 0;
    }
    if (!strcmp(argv[1], "-h")) {
        Help();
        return 0;
    }
    std::cout << "Invalid option" << std::endl;
    return 0;
}
