#include <unordered_set>

#include "cpu_info.hpp"

std::unordered_set<std::string> KNOWN_CPU_FEATURES = {
#define CPU_FEATURE_DEF(x) #x,
#include "cpu_features.def"
};
