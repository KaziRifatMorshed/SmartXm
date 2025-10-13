#ifndef VERDICT_H
#define VERDICT_H

#include <string>

struct Verdict {
    std::string verdict;
    int cpu_time;
    int memory_size;

    Verdict() : verdict(""), cpu_time(0), memory_size(0) {}
    Verdict(std::string verdict, int cpu_time, int memory_size) : verdict(verdict), cpu_time(cpu_time), memory_size(memory_size) {}
};

#endif // VERDICT_H
