#include <windows.h>
#include <iostream>
#include <vector>
#include<cstdint>

int main() {
    DWORD len = 0;
    if (!GetLogicalProcessorInformation(nullptr, &len) &&
        GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        std::cerr << "Failed to query processor info." << std::endl;
        return 1;
    }

    std::vector<uint8_t> buf(len);
    auto info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION>(buf.data());

    if (!GetLogicalProcessorInformation(info, &len)) {
        std::cerr << "Failed to get processor info." << std::endl;
        return 1;
    }

    unsigned int cores = 0;
    size_t count = len / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
    
    for (size_t i = 0; i < count; ++i) {
        if (info[i].Relationship == RelationProcessorCore) {
            ++cores;
        }
    }

    std::cout << "Physical cores: " << cores << std::endl;
    return 0;
}