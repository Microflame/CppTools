#pragma once

#include <string>

namespace vtools
{

struct Md5
{
    uint32_t a0;
    uint32_t b0;
    uint32_t c0;
    uint32_t d0;
    uint32_t block[16];
    uint8_t block_vacant;
    uint64_t total_size;

    void Init();
    void Update(const void* data, size_t size);
    void Finish();

private:
    void ProcessBlock();
};

void HexDigest(char* dest, const void* src, size_t src_size);
std::string HexDigest(const void* src, size_t src_size);

} // namespace vtools
