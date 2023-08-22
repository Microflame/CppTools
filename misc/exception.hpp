#pragma once

#include <string.h>

#include <string>
#include <stdexcept>


namespace vtools
{

#define VTOOLS_CONCAT_IMPL(a, b) a ## b
#define VTOOLS_CONCAT(a, b) VTOOLS_CONCAT_IMPL(a, b)

#define VTOOLS_STRINGIZE_IMPL(x) #x
#define VTOOLS_STRINGIZE(x) VTOOLS_STRINGIZE_IMPL(x)

#define VTOOLS_THROW(err)                                               \
    do {                                                                \
        std::string location = __FILE__":"                              \
                               VTOOLS_STRINGIZE(__LINE__) ": ";         \
        throw std::runtime_error(location + (err));                     \
    } while (0)

#define VTOOLS_THROW_IF_POSIX_ERR(x)                                    \
    do {                                                                \
        if (int64_t(x) == -1) {                                         \
            VTOOLS_THROW(ErrnoToString(errno));                         \
        }                                                               \
    } while (0)

#define VTOOLS_ASSERT(x)                                                \
    do {                                                                \
        if (!(x)) {                                                     \
            VTOOLS_THROW("(" #x ") was not fulfilled");                 \
        }                                                               \
    } while (0)

static std::string ErrnoToString(int errnum) {
    char ERROR_BUFFER[1024] = {}; // TODO: What is the correct size?
    ERROR_BUFFER[0] = 0; // TODO: This must be unnecessary
    const char* err_str = strerror_r(errnum, ERROR_BUFFER, sizeof(ERROR_BUFFER));

    return { err_str };
}

} // namespace vtools
