#ifdef _WIN32

#include "Server.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <mutex>
#include <thread>
#include <filesystem>
#include <ctime>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "networking/FileMeta.h"
#include "Message.h"

#pragma comment(lib, "ws2_32.lib")

bool Server::running = false;
Server* Server::serverInstance = nullptr;

/*
C:\Users\n00bCod3r-tiny10\Documents\GitHub\SmartXm\src\SmartXm-CSEKU\networking\server\server_windows.cpp:24: warning: overflow in conversion from 'SOCKET' {aka 'long long unsigned int'} to 'int' changes value from '18446744073709551615' to '-1' [-Woverflow]
..\..\networking\server\server_windows.cpp:24:72: warning: overflow in conversion from 'SOCKET' {aka 'long long unsigned int'} to 'int' changes value from '18446744073709551615' to '-1' [-Woverflow]
   24 |     : port(port_), secretKey(secret), status("NOT RUNNING"), server_fd(INVALID_SOCKET)
      |                                                                        ^~~~~~~~~~~~~~
 */

Server::Server(int port_, const std::string& secret)
    : port(port_), secretKey(secret), status("NOT RUNNING"), server_fd(INVALID_SOCKET)
{
    std::cout << "Local Server Constructor invoked (Windows)" << std::endl;
    localIP = fetchLocalIP();
    status = "CREATED";
    Server::start();
}

Server::~Server() {
    std::cout << "Server Destructor invoked" << std::endl;
    stop();
    running = false;
    serverInstance = nullptr;
    status = "DESTROYED";
}

Server* Server::createServer() {
    if (!running) {
        std::cout << "No server instance found, NEW server starting..." << std::endl;
        serverInstance = new Server();
        running = true;
    }
    std::cout << "Server instance exists" << std::endl;
    return Server::serverInstance;
}

int Server::start() {
    if (running) return 0;

    std::cout << "Starting server (Windows)..." << std::endl;
    WSADATA wsaData;
    int wsaErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaErr != 0) {
        std::cerr << "WSAStartup failed: " << wsaErr << std::endl;
        status = "FAILED";
        return 1;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == (int)INVALID_SOCKET) {
        std::cerr << "socket() failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        status = "FAILED";
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cerr << "setsockopt() failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        status = "FAILED";
        return 1;
    }

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(server_fd, (SOCKADDR*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        status = "FAILED";
        return 1;
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "listen() failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        status = "FAILED";
        return 1;
    }

    status = "RUNNING";
    running = true;

    std::cout << "====== SmartXm-CSEKU LOCAL SERVER (Windows) ======" << std::endl;
    std::cout << "Server will listen on port " << port << std::endl;
    std::cout << "Server local IP (give this to client): " << localIP << std::endl;

           // Start threads
    acceptThread = std::thread(&Server::acceptLoop, this);

    return 0;
}

void Server::stop() {
    if (!running) return;
    running = false;
    status = "STOPPED";
    std::cout << "----- STOPPING SERVER (Windows) -----" << std::endl;

           // Close the listening socket to unblock accept()
    closesocket(server_fd);

           // Close all client sockets
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto& ci : clients) {
        closesocket(ci.socfd);
    }
    clients.clear();

    WSACleanup();
    std::cout << "----- SERVER SHOULD BE STOPPED -----" << std::endl;
}

std::vector<ClientInfo> Server::getClients() {
    std::lock_guard<std::mutex> lock(clientsMutex);
    return clients;
}

std::string Server::getStatus() {
    return status;
}

std::string Server::getLocalIP() {
    return localIP;
}

void Server::acceptLoop() {
    while (running) {
        sockaddr_in client_addr = {};
        int client_addr_len = sizeof(client_addr);

        int client_socket = accept(server_fd, (SOCKADDR*)&client_addr, &client_addr_len);
        if (!running) break;
        if (client_socket == (int)INVALID_SOCKET) {
            if (serverInstance == nullptr) std::cerr << "failed to accept connection ???" << std::endl;
            continue;
        }

               // AUTHENTICATION:
        char auth_buffer[256] = {0};
        int received_length = recv(client_socket, auth_buffer, sizeof(auth_buffer) - 1, 0);
        if (received_length <= 0) {
            std::cerr << "failed to receive secret key" << std::endl;
            closesocket(client_socket);
            continue;
        }
        auth_buffer[received_length] = '\0';
        if (secretKey != std::string(auth_buffer)) {
            std::cout << secretKey << " " << std::string(auth_buffer) << "\n";
            std::cout << "Authentication failed for client from " << inet_ntoa(client_addr.sin_addr) << std::endl;
            closesocket(client_socket);
            continue;
        }

               // RECEIVE_CLIENT_INFO:
        char client_name[50] = {0};
        int name_len = recv(client_socket, client_name, sizeof(client_name) - 1, 0);
        if (name_len <= 0) {
            std::cerr << "failed to receive client name" << std::endl;
            closesocket(client_socket);
            continue;
        }
        client_name[name_len] = '\0';

               // Add client to list
        ClientInfo ci;
        ci.socfd = client_socket;
        ci.ip = inet_ntoa(client_addr.sin_addr);
        ci.port = ntohs(client_addr.sin_port);
        ci.clientName = client_name;
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(ci);
        }

        std::cout << "Client '" << client_name << "' connected from " << ci.ip << ":" << ci.port << std::endl;

        std::thread client_thread(&Server::handleClient, this, client_socket);
        client_thread.detach();
    }
}

void Server::handleClient(int client_socket) {
    Message msg;
    while (running) {
        int valread = recv(client_socket, (char*)&msg, sizeof(msg), 0);
        if (valread <= 0) {
            std::cout << "Client (socket_id=" << client_socket << ") disconnected!" << std::endl;
            break;
        }
        else {
            std::cout << "Received msg from '" << msg.sender_name << "': " << msg.text << std::endl;
        }
    }
    closesocket(client_socket);

    std::lock_guard<std::mutex> lock(clientsMutex);
    clients.erase(std::remove_if(clients.begin(), clients.end(),
                                 [client_socket](const ClientInfo& ci) { return ci.socfd == client_socket; }),
                  clients.end());

    std::cout << "Client Handler Thread Terminated!! socket id = " << client_socket << std::endl;
}

void Server::printClientsLoop(int intervalSeconds) {
    while (running) {
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            std::cout << "-----------------------\nConnected Clients:\n";
            for (const auto& ci : clients) {
                std::cout << "Client: " << ci.ip << ":" << ci.port << ", Name: " << ci.clientName << ", fd: " << ci.socfd << std::endl;
            }
            std::cout << "-----------------------" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
    }
}

std::string Server::fetchLocalIP() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed in fetchLocalIP" << std::endl;
        return "127.0.0.1"; // Default to localhost on failure
    }

    char hostName[256];
    if (gethostname(hostName, sizeof(hostName)) == SOCKET_ERROR) {
        std::cerr << "gethostname failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return "127.0.0.1";
    }

    hostent* hostEntry = gethostbyname(hostName);
    if (!hostEntry) {
        std::cerr << "gethostbyname failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return "127.0.0.1";
    }

    char* ipAddress = inet_ntoa(*((in_addr*)hostEntry->h_addr_list[0]));
    if (!ipAddress) {
        std::cerr << "inet_ntoa failed" << std::endl;
        WSACleanup();
        return "127.0.0.1";
    }

    std::string localIP = std::string(ipAddress);
    WSACleanup();
    return localIP;
}

// send to specific client
bool Server::sendFileToClient(int client_sock, const std::string path, const std::string msg) {
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
    return meta.send_on_socket(client_sock);
}

// Broadcast to all
bool Server::sendFileToAllClients(const FileMeta& meta) {
    std::vector<int> sockets;
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (const auto& c : clients) sockets.push_back(c.socfd);
    }
    bool all_ok = true;
    std::vector<std::thread> workers;
    for (int sock : sockets) {
        workers.emplace_back([&, sock]() {
            if (!meta.send_on_socket(sock)) {
                std::cerr << "Failed to send FileMeta to client socket: " << sock << "\n";
                all_ok = false;
            }
        });
    }
    for (auto& t : workers) t.detach();
    return all_ok;
}

// Receive from a client
FileMeta Server::receiveFileFromClient(int client_sock) {
    return FileMeta::recv_from_socket(client_sock);
}

#endif
