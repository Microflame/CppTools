#pragma once

#include <cstdint>

namespace vtools
{

int64_t GetTimeNanos();
int64_t GetTimeMicros();
int64_t GetTimeMillis();

/**
 * Returns time in seconds
*/
double GetTime();

struct Timer {
    int64_t ns = 0;

    Timer() { Start(); }
    void Start() { ns = -GetTimeNanos(); }

    int64_t Stop() {
        if (ns < 0) {
            ns += GetTimeNanos();
        }
        return ns;
    }

    int64_t Ns() const {
        if (ns < 0) {
            return GetTimeNanos() + ns;
        }
        return ns;
    }

    int64_t Us() const { return Ns() / 1'000; }
    int64_t Ms() const { return Ns() / 1'000'000; }
    double Get() const { return Ns() / 1e9; }
};

} // namespace vtools
