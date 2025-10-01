#ifdef _WIN32

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

Client::Client() : sock_fd(INVALID_SOCKET), connected(false) {}

Client::~Client() { disconnect(); }

void Client::disconnect() {
    std::lock_guard<std::mutex> lock(connMutex);
    if (connected) {
        connected = false;
        if (fileReceiverThread.joinable()) {
            fileReceiverThread.join();
        }
        closesocket(sock_fd);
        sock_fd = INVALID_SOCKET;
        WSACleanup();
    }
}

// Send file to server
bool send_file_to_server(SOCKET sock_fd, const std::string& path, const std::string& msg) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;
    file.seekg(0, std::ios::end);
    size_t sz = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> filedata(sz);
    file.read(filedata.data(), sz);

    std::string fname = std::filesystem::path(path).filename().string();
    std::string ext = std::filesystem::path(path).extension().string();
    if (ext.size() && ext[0] == '.') ext = ext.substr(1);

    FileMeta meta(fname, ext, std::time(nullptr), std::move(filedata), msg);
    return meta.send_on_socket(sock_fd);
}

// Receive file from server
FileMeta receive_file_from_server(SOCKET sock_fd) {
    return FileMeta::recv_from_socket(sock_fd);
}

void receive_file(SOCKET client_sock_fd) {
    try {
        FileMeta meta = FileMeta::recv_from_socket(client_sock_fd);
        std::string filename = meta.filename.empty() ? "received_file.bin" : meta.filename;
        std::ofstream outfile(filename, std::ios::binary);
        outfile.write(meta.file_data.data(), meta.file_data.size());
        outfile.close();
        std::cout << "Received file '" << filename << "' (" << meta.file_data.size() << " bytes)\n";
        if (!meta.message.empty()) {
            std::cout << "Server message: " << meta.message << "\n";
        }
    } catch (std::exception& e) {
        std::cerr << "File receive error: " << e.what() << std::endl;
    }
}

bool Client::connectToServer(const std::string &ip_addr, int port) {
    std::lock_guard<std::mutex> lock(connMutex);

    if (connected)
        return true;

    WSADATA wsaData;
    int wsaErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaErr != 0) {
        std::cerr << "WSAStartup failed: " << wsaErr << std::endl;
        return false;
    }

    sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_fd == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (InetPtonA(AF_INET, ip_addr.c_str(), &server_addr.sin_addr) != 1) {
        std::cerr << "Invalid address/ Address not supported\n";
        closesocket(sock_fd);
        WSACleanup();
        return false;
    }

    if (connect(sock_fd, (SOCKADDR *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection Failed: " << WSAGetLastError() << std::endl;
        closesocket(sock_fd);
        WSACleanup();
        return false;
    } else {
        std::cout << "Connected to server at: " << ip_addr << " : " << port << std::endl;
    }

           // Authenticate with secret key
    if (send(sock_fd, CLIENT_SECRET_KEY, strlen(CLIENT_SECRET_KEY), 0) < 0) {
        std::cerr << "Authentication failed" << std::endl;
    } else {
        std::cout << "Server Authentication Successful" << std::endl;
    }

           // Send client name to server
    std::string clientIdentity = Server::fetchLocalIP();
    if (send(sock_fd, clientIdentity.c_str(), (int)clientIdentity.length(), 0) < 0) {
        std::cerr << "Client info sent failed" << std::endl;
    }

    connected = true;

           // Start file receiving thread (optional)
    std::thread t(receive_file, sock_fd);
    t.detach();

    return true;
}

#endif
