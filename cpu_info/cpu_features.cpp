#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>

#include "cpu_info.hpp"

std::unordered_set<std::string> KNOWN_CPU_FEATURES = {
    "avx",
    "avx2",
};

int main() {
    vtools::GetCpuInfo();
}
