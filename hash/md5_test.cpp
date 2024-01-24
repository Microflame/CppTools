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

template <typename T>
T Min(T a, T b) { return a < b ? a : b; }

void PrintFileHash(const char* path) {
    FILE* file = fopen64(path, "rb");
    VTOOLS_ASSERT(file);
    VTOOLS_DEFER { fclose(file); };

    fseeko64(file, 0, SEEK_END);
    int64_t file_size = ftello64(file);
    VTOOLS_ASSERT(file_size > 0);
    fseeko64(file, 0, SEEK_SET);

    constexpr int64_t buffer_size = 4096;
    uint8_t buffer[buffer_size];

    vtools::Md5 md5;
    md5.Init();
    for (int64_t i = 0; i < file_size; i += buffer_size) {
        int64_t size = Min(buffer_size, file_size - i);
        fread(buffer, 1, size, file);
        md5.Update(buffer, size);
    }
    md5.Finish();
    
    std::string hash = vtools::HexDigest(&md5.state, sizeof(md5.state));
    std::cout << "Hash: " << hash << '\n';
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Wrong argc\n";
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "file") {
        PrintFileHash(argv[2]);
    } else if (mode == "raw") {
        std::string data = argv[2];
        std::string hash = vtools::CalcMd5(data.data(), data.size());
        std::cout << "Hash: " << hash << "\n";
    } else {
        std::cerr << "Wrong mode\n";
        return 1;
    }

    return 0;
}
