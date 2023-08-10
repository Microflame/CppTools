#pragma once

#include <unordered_set>
#include <string>

namespace vtools {

extern std::unordered_set<std::string> KNOWN_CPU_FEATURES;

std::unordered_set<std::string> GetAvailableCpuFeatures(int proc_id);

} // namespace vtools
