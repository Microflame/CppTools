#include "time.hpp"

#include <chrono>

namespace vtools
{

using StdClock = std::chrono::high_resolution_clock;

int64_t GetTimeNanos() {
    auto since_epoch = StdClock::now().time_since_epoch();
    int64_t count = std::chrono::duration_cast<std::chrono::nanoseconds>(since_epoch).count();
    return count;
}

int64_t GetTimeMicros() { return GetTimeNanos() / 1'000; }
int64_t GetTimeMillis() { return GetTimeNanos() / 1'000'000; }
double GetTime() { return GetTimeNanos() / 1e9; }

} // namespace vtools
