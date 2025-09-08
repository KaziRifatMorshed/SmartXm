#include "Client.h"
#ifdef __linux__
#include "client_linux.cpp"
#elif _WIN32
#include "client_windows.cpp"
#endif
