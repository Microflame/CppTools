#pragma once

#include <vector>
#include <string>

namespace vtools
{

struct CpuInfo {
    uint32_t processor = -1;
    std::string vendor_id;
    int32_t cpu_family = -1;
    uint32_t model = -1;
    std::string model_name;
    int32_t stepping = -1;
    uint32_t microcode = -1;
    float cpu_mhz = -1;
    uint32_t cache_size_kb = -1;

    int32_t physical_id = -1;
    int32_t siblings = -1;
    int32_t core_id = -1;
    int32_t cpu_cores = -1;
    int32_t apicid = -1;
    int32_t initial_apicid = -1;

    int32_t cpuid_level = -1;

    std::vector<std::string> flags;
    std::vector<std::string> bugs;

    float bogomips = -1;
    int32_t tlb_size = -1;
    uint32_t clflush_size = -1;
    int32_t cache_alignment = -1;
    uint32_t physical_address_bits = -1;
    uint32_t virtual_address_bits = -1;
};

std::vector<CpuInfo> GetCpuInfo();

} // namespace vtools
