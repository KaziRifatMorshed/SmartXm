#pragma once
#ifndef TERMINALEXECUTER_H
#define TERMINALEXECUTER_H

#include <cstdio>
#include <stdexcept>
#include <string>
#include <array>

// `static` saved the LINKER ERROR
static std::string termiExec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
#ifdef _WIN32
    FILE* pipe = _popen(cmd, "r");
#else
    FILE* pipe = popen(cmd, "r");
#endif
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
    } catch (...) {
#ifdef _WIN32
        _pclose(pipe);
#else
        pclose(pipe);
#endif
        throw;
    }
#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
    return result;
}
#endif // TERMINALEXECUTER_H
