#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <map>

namespace vtools
{

struct HttpRequestHeader {
    std::string method;
    std::vector<std::string> path;
    std::map<std::string, std::string> parameters;
    std::map<std::string, std::string> headers;
};

struct HttpResponseHeader {
    int status;
    std::string descr;
    std::map<std::string, std::string> headers;
};

std::string UrlDecode(std::string_view str);

bool ReadHttpRequestHeader(const char* data, size_t size, HttpRequestHeader& dest);
std::string RenderHttpResponseHeader(const HttpResponseHeader& header);

} // namespace vtools
