#ifdef __linux__
#include "server_linux.cpp"
#elif _WIN32
#include "server_windows.cpp"
#endif
