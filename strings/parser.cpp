#include "parser.hpp"

#include <string>

namespace vtools
{

StringParser::StringParser(std::string_view str) :
  start_(str.data()),
  end_(str.data() + str.size()) {}

size_t StringParser::Size() const {
    return end_ - start_;
}

std::string_view StringParser::Get() const {
    return std::string_view(start_, size_t(end_ - start_));
}

std::vector<std::string_view> StringParser::GetSplit(char ch) const {
    std::vector<std::string_view> res;
    StringParser parser = Get();
    while (parser.Size() > 0) {
        res.push_back(parser.Find(ch));
        parser.SkipChar(ch);
    }
    return res;
}

const char* StringParser::Next() {
    const char* old_start = start_;
    start_ += start_ < end_;
    return old_start;
}

std::string_view StringParser::Find(char ch) {
    const char* old_start = start_;
    while (start_ < end_ && *start_ != ch) {
        ++start_;
    }
    return std::string_view(old_start, start_ - old_start);
}

std::string_view StringParser::Find(std::string_view str) {
    std::string_view cur(start_, size_t(end_ - start_));
    size_t pos = cur.find(str);
    if (pos == std::string::npos) {
        return {};
    }

    const char* old_start = start_;
    start_ += pos;
    return std::string_view(old_start, pos);
}

std::string_view StringParser::FindNot(char ch) {
    const char* old_start = start_;
    while (start_ < end_ && *start_ == ch) {
        ++start_;
    }
    return std::string_view(old_start, start_ - old_start);
}

int64_t StringParser::ReadInt(int base) {
    size_t offset = 0;
    std::string content(start_, end_);
    int64_t res = std::stoll(content, &offset, base);
    start_ += offset;
    return res;
}

uint64_t StringParser::ReadUint(int base) {
    size_t offset = 0;
    std::string content(start_, end_);
    uint64_t res = std::stoul(content, &offset, base);
    start_ += offset;
    return res;
}

double StringParser::ReadDouble() {
    size_t offset = 0;
    std::string content(start_, end_);
    double res = std::stold(content, &offset);
    start_ += offset;
    return res;
}

bool StringParser::SkipCount(size_t count) {
    if (Size() < count) {
        return false;
    }
    start_ += count;
    return true;
}

bool StringParser::SkipStr(std::string_view str) {
    if (str.size() > (end_ - start_)) {
        return false;
    }

    const char* s = start_;
    for (char ch: str) {
        if (ch != *s) {
            return false;
        }
        ++s;
    }
    start_ = s;
    return true;
}

bool StringParser::SkipChar(char ch) {
    if (start_ == end_ || *start_ != ch) {
        return false;
    }
    ++start_;
    return true;
}
    
} // namespace vtools
