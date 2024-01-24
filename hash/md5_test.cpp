#include <stdio.h>

#include <iostream>
#include <string>

#include "md5.hpp"
#include "misc/exception.hpp"
#include "misc/defer.hpp"

std::string ReadFile(const char* path) {
    FILE* file = fopen(path, "rb");
    VTOOLS_ASSERT(file);
    VTOOLS_DEFER { fclose(file); };

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    VTOOLS_ASSERT(size > 0);
    fseek(file, 0, SEEK_SET);
    std::string res(size, '\0');
    fread(res.data(), 1, res.size(), file);
    return res;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Wrong argc\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string data;
    if (mode == "file") {
        data = ReadFile(argv[2]);
    } else {
        data = argv[2];
    }

    std::string hash = vtools::CalcMd5(data.data(), data.size());
    std::cout << "Hash: " << hash << "\n";
    return 0;
}
