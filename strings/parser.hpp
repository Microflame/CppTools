#pragma once

#include <string_view>

namespace vtools
{

class StringParser {
public:
    StringParser(std::string_view str);

    const char* Next();
    std::string_view Find(char ch);
    std::string_view FindNot(char ch);
    int64_t ReadInt(int base = 10);
    uint64_t ReadUint(int base = 10);
    double ReadDouble();
    bool Skip(std::string_view str);

private:
    const char* start_;
    const char* end_;
};

} // namespace vtools
