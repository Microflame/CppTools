add_library(CpuInfo
    cpu_features.cpp
    cpu_info.cpp
)

add_executable(CpuInfoPerf cpu_info_perf.cpp)
target_link_libraries(CpuInfoPerf PRIVATE CpuInfo Time)
