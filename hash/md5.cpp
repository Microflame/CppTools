#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "md5.hpp"


namespace vtools
{

namespace
{

uint8_t shifts[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
};

uint32_t K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

uint32_t rotate_left(uint32_t x, uint32_t shift)
{
    return (x << shift) | (x >> (sizeof(x) - shift));
}

} // namespace


void Md5::Init() {
    a0 = 0x67452301;
    b0 = 0xefcdab89;
    c0 = 0x98badcfe;
    d0 = 0x10325476;
    block_vacant = sizeof(block);
    total_size = 0;
}

void Md5::ProcessBlock() {
    assert(block_vacant == 0);
    uint32_t A = a0;
    uint32_t B = b0;
    uint32_t C = c0;
    uint32_t D = d0;

    for (uint8_t i = 0; i < 64; i++) {
        uint32_t F;
        uint32_t g;
        if (i < 16) {
            F = (B & C) | (~B & D);
            g = i;
        } else if (i < 32) {
            F = (D & B) | (~D & C);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            F = B ^ C ^ D;
            g = (3 * i + 5) % 16;
        } else {
            F = C ^ (B | ~D);
            g = (7 * i) % 16;
        }
        F += A + K[i] + block[g];
        A = D;
        D = C;
        C = B;
        B = B + rotate_left(F, shifts[i]);
    }

    a0 += A;
    b0 += B;
    c0 += C;
    d0 += D;

    block_vacant = sizeof(block);
}

void Md5::Update(const void* data_as_void_ptr, size_t size) {
    const uint8_t* data = static_cast<const uint8_t*>(data_as_void_ptr);
    uint8_t* block_bytes = reinterpret_cast<uint8_t*>(block);

    while (size >= block_vacant) {
        memcpy(block_bytes + sizeof(block) - block_vacant, data, block_vacant);
        data += block_vacant;
        size -= block_vacant;

        ProcessBlock();
    }

    assert(block_vacant > size);
    if (size) {
        memcpy(block_bytes + sizeof(block) - block_vacant, data, size);
        block_vacant -= size;
    }

    total_size += size;
}

void Md5::Finish() {
    uint8_t* block_bytes = reinterpret_cast<uint8_t*>(block);

    assert(block_vacant > 0);
    {
        block_bytes[sizeof(block) - block_vacant] = 0x80;
        block_vacant -= 1;
    }

    if (block_vacant < 8)
    {
        memset(block_bytes + sizeof(block) - block_vacant, 0, block_vacant);
        ProcessBlock();
    }

    assert(block_vacant >= 8);
    {
        uint64_t total_size_in_bits = total_size * 8;
        uint8_t num_pad = block_vacant - 8;
        memset(block_bytes + sizeof(block) - block_vacant, 0, num_pad);
        uint32_t* total_size_in_bits_parts = reinterpret_cast<uint32_t*>(&total_size_in_bits);
        block[14] = total_size_in_bits_parts[0];
        block[15] = total_size_in_bits_parts[1];
        ProcessBlock();
    }
}


void HexDigest(char* dest, const void* src_as_void_ptr, size_t src_size) {
    const uint8_t* src = static_cast<const uint8_t*>(src_as_void_ptr);

    for (size_t dest_idx = 0; dest_idx < src_size * 2; dest_idx++) {
        size_t src_idx = dest_idx / 2;
        uint8_t num = src[src_idx];
        num = src_idx % 2 ? num & 0xF : num >> 4;
        dest[dest_idx] = num < 10 ? ('0' + num) : ('a' + num - 10);
    }
}

std::string HexDigest(const void* src, size_t src_size) {
    std::string res(src_size * 2, '\0');
    HexDigest(res.data(), src, src_size);
    return res;
}


} // namespace vtools
