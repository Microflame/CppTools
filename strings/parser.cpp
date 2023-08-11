#include "parser.hpp"

#include <string>

namespace vtools
{

StringParser::StringParser(std::string_view str) :
  start_(str.data()),
  end_(str.data() + str.size()) {

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

bool StringParser::Skip(std::string_view str) {
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
    
} // namespace vtools
