#pragma once

#include <string_view>
#include <vector>

namespace vtools
{

class StringParser {
public:
    StringParser(std::string_view str);

    size_t Size() const;
    std::string_view Get() const;
    std::vector<std::string_view> GetSplit(char ch) const;

    const char* Next();
    std::string_view Find(char ch);
    std::string_view Find(std::string_view str);
    std::string_view FindNot(char ch);
    int64_t ReadInt(int base = 10);
    uint64_t ReadUint(int base = 10);
    double ReadDouble();
    bool SkipCount(size_t count);
    bool SkipStr(std::string_view str);
    bool SkipChar(char ch);

private:
    const char* start_;
    const char* end_;
};

} // namespace vtools
