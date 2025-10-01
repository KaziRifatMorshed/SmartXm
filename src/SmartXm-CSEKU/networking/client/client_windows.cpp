#ifdef _WIN32

#define DEBUG_ON

#include "./../server/Server.h"
#include "Client.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <networking/FileMeta.h>

#pragma comment(lib, "ws2_32.lib")

Client *Client::clientInstance = nullptr;
std::time_t Client::lastLoginTime = 0;

Client::Client() : sock_fd(INVALID_SOCKET), connected(false) {
#ifdef DEBUG_ON
    std::cout << "[Client] Client object created." << std::endl;
#endif
}

Client::~Client() {
#ifdef DEBUG_ON
    std::cout << "[Client] Client object destroyed." << std::endl;
#endif
    disconnect();
}

void Client::disconnect() {
    std::lock_guard<std::mutex> lock(connMutex);
    if (connected) {
#ifdef DEBUG_ON
        std::cout << "[Client::disconnect] Disconnecting from server." << std::endl;
#endif
        connected = false;
        if (fileReceiverThread.joinable()) {
#ifdef DEBUG_ON
            std::cout << "[Client::disconnect] Joining file receiver thread." << std::endl;
#endif
            fileReceiverThread.join();
        }
        closesocket(sock_fd);
        sock_fd = INVALID_SOCKET;
        WSACleanup();
#ifdef DEBUG_ON
        std::cout << "[Client::disconnect] Disconnected successfully." << std::endl;
#endif
    }
}

// Send file to server
bool send_file_to_server(SOCKET sock_fd, const std::string& path, const std::string& msg) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
#ifdef DEBUG_ON
        std::cerr << "[send_file_to_server] Failed to open file: " << path << std::endl;
#endif
        return false;
