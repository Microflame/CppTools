#include "cpu_info.hpp"

#include <regex>
#include <fstream>
#include <sstream>

#include <iostream>

namespace vtools
{

void TryParseField(uint32_t& field, const std::string& line, const std::regex& re, std::smatch& match) {
    if (std::regex_match(line, match, re) && match.size() == 2) {
        field = std::stoul(match[1]);
    }
}

void TryParseField(int32_t& field, const std::string& line, const std::regex& re, std::smatch& match) {
    if (std::regex_match(line, match, re) && match.size() == 2) {
        field = std::stoi(match[1]);
    }
}

void TryParseField(float& field, const std::string& line, const std::regex& re, std::smatch& match) {
    if (std::regex_match(line, match, re) && match.size() == 2) {
        field = std::stof(match[1]);
    }
}

void TryParseField(std::string& field, const std::string& line, const std::regex& re, std::smatch& match) {
    if (std::regex_match(line, match, re) && match.size() == 2) {
        field = match[1];
    }
}

void TryParseField(std::vector<std::string>& field, const std::string& line, const std::regex& re, std::smatch& match) {
    if (std::regex_match(line, match, re) && match.size() == 2) {
        std::string flag;
        std::stringstream flags(match[1]);
        while (std::getline(flags, flag, ' ')) {
            field.push_back(flag);
        }
    }
}

std::vector<CpuInfo> GetCpuInfo() {
    std::vector<CpuInfo> res;

    std::ifstream ifs("/proc/cpuinfo");
    if (!ifs) {
        throw std::runtime_error("Can not open cpuinfo\n");
    }

    CpuInfo cpu_info = {};

    std::string re_uint = "\\s*:\\s*(\\d*).*";
    std::string re_int = "\\s*:\\s*(-?\\d*).*";
    std::string re_float = "\\s*:\\s*(\\d*\\.\\d*).*";
    std::string re_str = "\\s*:\\s*(.*)";

    std::regex re_processor("processor" + re_uint);
    std::regex re_vendor_id("vendor_id" + re_str);
    std::regex re_cpu_family("cpu family" + re_int);
    std::regex re_model("model" + re_uint);
    std::regex re_model_name("model name" + re_str);
    std::regex re_stepping("stepping" + re_int);
    std::regex re_microcode("microcode" + re_uint);
    std::regex re_cpu_mhz("cpu MHz" + re_float);
    std::regex re_cache_size_kb("cache size" + re_uint);
    std::regex re_physical_id("physical id" + re_int);
    std::regex re_siblings("siblings" + re_int);
    std::regex re_core_id("core id" + re_int);
    std::regex re_cpu_cores("cpu cores" + re_int);
    std::regex re_apicid("apicid" + re_int);
    std::regex re_initial_apicid("initial apicid" + re_int);
    std::regex re_cpuid_level("cpuid level" + re_int);
    std::regex re_flags("flags" + re_str);
    std::regex re_bugs("bugs" + re_str);
    std::regex re_bogomips("bogomips" + re_float);
    std::regex re_tlb_size("TLB size" + re_int);
    std::regex re_clflush_size("clflush size" + re_uint);
    std::regex re_cache_alignment("cache_alignment" + re_int);

    std::regex re_address_sizes("physical_address_bits\\s*:\\s* (\\d*) bits physical, (\\d*) bits virtual.*");

    std::smatch match;

    std::string line;
    while (std::getline(ifs, line)) {
        if (line.size() == 0) {
            res.push_back(std::move(cpu_info));
            cpu_info = {};
            continue;
        }

        TryParseField(cpu_info.processor, line, re_processor, match);
        TryParseField(cpu_info.vendor_id, line, re_vendor_id, match);
        TryParseField(cpu_info.cpu_family, line, re_cpu_family, match);
        TryParseField(cpu_info.model, line, re_model, match);
        TryParseField(cpu_info.model_name, line, re_model_name, match);
        TryParseField(cpu_info.stepping, line, re_stepping, match);
        TryParseField(cpu_info.microcode, line, re_microcode, match);
        TryParseField(cpu_info.cpu_mhz, line, re_cpu_mhz, match);
        TryParseField(cpu_info.cache_size_kb, line, re_cache_size_kb, match);
        TryParseField(cpu_info.physical_id, line, re_physical_id, match);
        TryParseField(cpu_info.siblings, line, re_siblings, match);
        TryParseField(cpu_info.core_id, line, re_core_id, match);
        TryParseField(cpu_info.cpu_cores, line, re_cpu_cores, match);
        TryParseField(cpu_info.apicid, line, re_apicid, match);
        TryParseField(cpu_info.initial_apicid, line, re_initial_apicid, match);
        TryParseField(cpu_info.cpuid_level, line, re_cpuid_level, match);
        TryParseField(cpu_info.flags, line, re_flags, match);
        TryParseField(cpu_info.bugs, line, re_bugs, match);
        TryParseField(cpu_info.bogomips, line, re_bogomips, match);
        TryParseField(cpu_info.tlb_size, line, re_tlb_size, match);
        TryParseField(cpu_info.clflush_size, line, re_clflush_size, match);
        TryParseField(cpu_info.cache_alignment, line, re_cache_alignment, match);

        if (std::regex_match(line, match, re_address_sizes) && match.size() == 3) {
            cpu_info.physical_address_bits = std::stoul(match[1]);
            cpu_info.virtual_address_bits = std::stoul(match[2]);
        }
    }

    return res;
}


} // namespace vtools


int main() {
    vtools::GetCpuInfo();
    return 0;
}
