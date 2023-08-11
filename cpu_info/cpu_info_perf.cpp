#include <iostream>

#include "cpu_info.hpp"
#include "time/time.hpp"


int main(int argc, char* argv[]) {
    vtools::GetCpuInfo();

    size_t num_runs = 1000;
    vtools::Timer t;
    for (size_t i = 0; i < num_runs; i++) {
        vtools::GetCpuInfo(); // TODO: prevent optimization
    }
    t.Stop();

    std::cout << "Runs: " << num_runs << "\n";
    std::cout << "Total time: " << t.Get() << " s\n";
    std::cout << "Time per run: " << (t.Ns() / num_runs) << " ns\n";

    return 0;
}
