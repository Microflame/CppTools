#include "cpu_info.hpp"

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <cassert>

#include <strings/parser.hpp>

namespace vtools
{

void ParseField(uint32_t& field, std::string_view value, int base = 10) {
    field = std::stoul(std::string(value), nullptr, base);
}

void ParseField(int32_t& field, std::string_view value) {
    field = std::stoi(std::string(value));
}

void ParseField(float& field, std::string_view value) {
    field = std::stof(std::string(value));
}

void ParseField(std::string& field, std::string_view value) {
    field = std::string(value);
}

void ParseField(std::vector<std::string>& field, std::string_view value) {
    std::string flag;
    std::stringstream flags((std::string(value)));
    while (std::getline(flags, flag, ' ')) {
        field.push_back(flag);
    }
}

std::vector<CpuInfo> GetCpuInfo() {
    std::vector<CpuInfo> res;

    std::ifstream ifs("/proc/cpuinfo");
    if (!ifs) {
        throw std::runtime_error("Can not open cpuinfo\n");
    }

    std::unordered_map<std::string, std::function<void(CpuInfo&, std::string_view)>> parsers;
    parsers["processor"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.processor, value); };
    parsers["vendor_id"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.vendor_id, value); };
    parsers["cpu family"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.cpu_family, value); };
    parsers["model"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.model, value); };

    parsers["model name"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.model_name, value); };
    parsers["stepping"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.stepping, value); };
    parsers["microcode"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.microcode, value, 16); };
    parsers["cpu MHz"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.cpu_mhz, value); };
    parsers["cache size"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.cache_size_kb, value); };
    parsers["physical id"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.physical_id, value); };
    parsers["siblings"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.siblings, value); };
    parsers["core id"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.core_id, value); };
    parsers["cpu cores"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.cpu_cores, value); };
    parsers["apicid"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.apicid, value); };
    parsers["initial apicid"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.initial_apicid, value); };
    parsers["cpuid level"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.cpuid_level, value); };
    parsers["flags"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.flags, value); };
    parsers["bugs"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.bugs, value); };
    parsers["bogomips"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.bogomips, value); };
    parsers["TLB size"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.tlb_size_4k_pages, value); };
    parsers["clflush size"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.clflush_size, value); };
    parsers["cache_alignment"] = [](CpuInfo& ci, std::string_view value){ ParseField(ci.cache_alignment, value); };

    parsers["address sizes"] = [](CpuInfo& ci, std::string_view value){
        StringParser parser(value);
        ci.physical_address_bits = parser.ReadUint();
        bool skipped = parser.Skip(" bits physical, ");
        assert(skipped);
        ci.virtual_address_bits = parser.ReadUint();
    };

    CpuInfo cpu_info = {};
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.size() == 0) {
            res.push_back(std::move(cpu_info));
            cpu_info = {};
            continue;
        }

        StringParser sp(line);
        std::string key = std::string(sp.Find('\t'));
        sp.Find(' ');
        sp.Next();
        std::string_view value = sp.Find('\0');

        auto it = parsers.find(key);
        if (it != parsers.end()) {
            it->second(cpu_info, value);
        }
    }

    return res;
}


} // namespace vtools
