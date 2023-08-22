#include "http.hpp"

#include <sstream>

#include <strings/parser.hpp>
#include <misc/exception.hpp>

namespace vtools
{

static constexpr char* HEADER_SEP = "\r\n";
static constexpr char* HEADER_TAIL = "\r\n\r\n";

static int ParseHexDigit(char ch) {
    if ('A' <= ch && ch <= 'F') {
        return 10 + ch - 'A';
    }
    if ('0' <= ch && ch <= '9') {
        return ch - '0';
    }
    return -1;
}

std::string UrlDecode(std::string_view str) {
    std::string res(str.size(), '\0');
    size_t w_pos = 0;
    size_t r_pos = 0;

    while (r_pos < str.size()) {
        const char r_ch = str[r_pos];
        char w_ch = r_ch;
        size_t num_read = 1;
        if (r_pos + 2 < str.size() && str[r_pos] == '%') {
            int hi = ParseHexDigit(str[r_pos + 1]);
            int lo = ParseHexDigit(str[r_pos + 2]);
            if (hi != -1 && lo != -1) {
                w_ch = hi * 16 + lo;
                num_read = 3;
            }
        }
        res[w_pos] = w_ch;
        w_pos += 1;
        r_pos += num_read;
    }
    res.resize(w_pos);
    return res;
}

bool ReadHttpRequestHeader(const char* data, size_t size, HttpRequestHeader& dest) {
    size_t header_tail_pos = 0;
    {
        std::string_view data_sv(data, size);
        std::string_view header_tail(HEADER_TAIL);
        header_tail_pos = data_sv.find(header_tail);
        if (header_tail_pos == std::string::npos) {
            return false;
        }
    }

    StringParser header_parser({data, header_tail_pos + 2});

    {
        StringParser first_line = header_parser.Find(HEADER_SEP);
        header_parser.SkipCount(2);
        VTOOLS_ASSERT(first_line.Size());
        dest.method = first_line.Find(' ');
        first_line.SkipCount(1);

        {
            StringParser url = first_line.Find(' ');
            url.SkipCount(1);
            first_line.SkipChar('/');

            StringParser path_parser = url.Find('?');
            VTOOLS_ASSERT(path_parser.SkipChar('/'));
            auto path = path_parser.GetSplit('/');
            dest.path = { path.begin(), path.end() };

            StringParser qs_parser = url;
            auto qs_parts = qs_parser.GetSplit('&');
            for (StringParser qs_part: qs_parts) {
                std::string_view key = qs_part.Find('=');
                qs_part.SkipCount(1);
                std::string key_decoded = UrlDecode(key);

                std::string_view value = qs_part.Get();
                std::string value_decoded = UrlDecode(value);
                dest.parameters[key_decoded] = value_decoded;
            }
        }
    }

    while (header_parser.Size() > 0) {
        StringParser header_line = header_parser.Find(HEADER_SEP);
        header_parser.SkipCount(2);
        VTOOLS_ASSERT(header_line.Size());

        StringParser key = header_line.Find(':');
        VTOOLS_ASSERT(key.Size());
        StringParser value = header_line;
        value.SkipCount(1);
        VTOOLS_ASSERT(value.Size());
        dest.headers[std::string(key.Get())] = value.Get();
    }

    return true;
}

std::string RenderHttpResponseHeader(const HttpResponseHeader& header) {
    std::stringstream ss;
    ss << header.status << ' ' << header.descr << HEADER_SEP;
    for (const auto& [k, v]: header.headers) {
        ss << k << ": " << v << HEADER_SEP;
    }
    ss << HEADER_SEP;
    return ss.str();
}

} // namespace vtools
