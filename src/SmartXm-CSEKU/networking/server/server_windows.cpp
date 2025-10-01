#define DEBUG_ON
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
#ifdef DEBUG_ON
    std::cout << "[Server] Constructor invoked (Windows)" << std::endl;
#endif
    localIP = fetchLocalIP();
    status = "CREATED";
    Server::start();
#ifdef DEBUG_ON
    std::cout << "[Server] Constructor completed" << std::endl;
#endif
}

Server::~Server() {
#ifdef DEBUG_ON
    std::cout << "[~Server] Destructor invoked" << std::endl;
#endif
    stop();
    running = false;
    serverInstance = nullptr;
    status = "DESTROYED";
#ifdef DEBUG_ON
    std::cout << "[~Server] Destructor completed" << std::endl;
#endif
}

Server* Server::createServer() {
#ifdef DEBUG_ON
    std::cout << "[createServer] Invoked" << std::endl;
#endif
    if (!running) {
        std::cout << "No server instance found, NEW server starting..." << std::endl;
        serverInstance = new Server();
        running = true;
    }
    std::cout << "Server instance exists" << std::endl;
#ifdef DEBUG_ON
    std::cout << "[createServer] Completed" << std::endl;
#endif
    return Server::serverInstance;
}

int Server::start() {
#ifdef DEBUG_ON
    std::cout << "[start] Invoked" << std::endl;
#endif
    if (running) return 0;

    std::cout << "Starting server (Windows)..." << std::endl;
    WSADATA wsaData;
    int wsaErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaErr != 0) {
        std::cerr << "WSAStartup failed: " << wsaErr << std::endl;
        status = "FAILED";
#ifdef DEBUG_ON
        std::cerr << "[start] WSAStartup failed" << std::endl;
#endif
        return 1;
    }
#ifdef DEBUG_ON
    std::cout << "[start] WSAStartup successful" << std::endl;
#endif

    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == (int)INVALID_SOCKET) {
        std::cerr << "socket() failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        status = "FAILED";
#ifdef DEBUG_ON
        std::cerr << "[start] Socket creation failed" << std::endl;
#endif
        return 1;
    }
#ifdef DEBUG_ON
    std::cout << "[start] Socket creation successful" << std::endl;
#endif

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cerr << "setsockopt() failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        status = "FAILED";
#ifdef DEBUG_ON
        std::cerr << "[start] setsockopt failed" << std::endl;
#endif
        return 1;
    }
#ifdef DEBUG_ON
    std::cout << "[start] setsockopt successful" << std::endl;
#endif

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(server_fd, (SOCKADDR*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        status = "FAILED";
#ifdef DEBUG_ON
        std::cerr << "[start] Bind failed" << std::endl;
#endif
        return 1;
    }
#ifdef DEBUG_ON
    std::cout << "[start] Bind successful" << std::endl;
#endif

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "listen() failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        status = "FAILED";
#ifdef DEBUG_ON
        std::cerr << "[start] Listen failed" << std::endl;
#endif
        return 1;
    }
#ifdef DEBUG_ON
    std::cout << "[start] Listen successful" << std::endl;
#endif

    status = "RUNNING";
    running = true;

    std::cout << "====== SmartXm-CSEKU LOCAL SERVER (Windows) ======" << std::endl;
    std::cout << "Server will listen on port " << port << std::endl;
    std::cout << "Server local IP (give this to client): " << localIP << std::endl;

           // Start threads
    acceptThread = std::thread(&Server::acceptLoop, this);
#ifdef DEBUG_ON
    std::cout << "[start] Accept thread started" << std::endl;
    std::cout << "[start] Completed" << std::endl;
#endif

    return 0;
}

void Server::stop() {
#ifdef DEBUG_ON
    std::cout << "[stop] Invoked" << std::endl;
#endif
    if (!running) return;
    running = false;
    status = "STOPPED";
    std::cout << "----- STOPPING SERVER (Windows) -----" << std::endl;

           // Close the listening socket to unblock accept()
    closesocket(server_fd);
#ifdef DEBUG_ON
    std::cout << "[stop] Listening socket closed" << std::endl;
#endif

           // Close all client sockets
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (auto& ci : clients) {
            closesocket(ci.socfd);
        }
        clients.clear();
    }
#ifdef DEBUG_ON
    std::cout << "[stop] All client sockets closed and cleared" << std::endl;
#endif

    WSACleanup();
    std::cout << "----- SERVER SHOULD BE STOPPED -----" << std::endl;
#ifdef DEBUG_ON
    std::cout << "[stop] WSACleanup called" << std::endl;
    std::cout << "[stop] Completed" << std::endl;
#endif
}

std::vector<ClientInfo> Server::getClients() {
#ifdef DEBUG_ON
    std::cout << "[getClients] Invoked" << std::endl;
#endif
    std::lock_guard<std::mutex> lock(clientsMutex);
#ifdef DEBUG_ON
    std::cout << "[getClients] Completed" << std::endl;
#endif
    return clients;
}

std::string Server::getStatus() {
#ifdef DEBUG_ON
    std::cout << "[getStatus] Invoked" << std::endl;
    std::cout << "[getStatus] Completed" << std::endl;
#endif
    return status;
}

std::string Server::getLocalIP() {
#ifdef DEBUG_ON
    std::cout << "[getLocalIP] Invoked" << std::endl;
    std::cout << "[getLocalIP] Completed" << std::endl;
#endif
    return localIP;
}

void Server::acceptLoop() {
#ifdef DEBUG_ON
    std::cout << "[acceptLoop] Invoked" << std::endl;
#endif
    while (running) {
        sockaddr_in client_addr = {};
        int client_addr_len = sizeof(client_addr);

        int client_socket = accept(server_fd, (SOCKADDR*)&client_addr, &client_addr_len);
        if (!running) break;
        if (client_socket == (int)INVALID_SOCKET) {
            if (serverInstance == nullptr) std::cerr << "failed to accept connection ???" << std::endl;
            continue;
        }
#ifdef DEBUG_ON
        std::cout << "[acceptLoop] Accepted new connection" << std::endl;
#endif

               // AUTHENTICATION:
        char auth_buffer[256] = {0};
        int received_length = recv(client_socket, auth_buffer, sizeof(auth_buffer) - 1, 0);
        if (received_length <= 0) {
            std::cerr << "failed to receive secret key" << std::endl;
            closesocket(client_socket);
#ifdef DEBUG_ON
            std::cerr << "[acceptLoop] Failed to receive secret key" << std::endl;
#endif
            continue;
        }
        auth_buffer[received_length] = '\0';
        if (secretKey != std::string(auth_buffer)) {
            std::cout << secretKey << " " << std::string(auth_buffer) << "\n";
            std::cout << "Authentication failed for client from " << inet_ntoa(client_addr.sin_addr) << std::endl;
            closesocket(client_socket);
#ifdef DEBUG_ON
            std::cerr << "[acceptLoop] Authentication failed" << std::endl;
#endif
            continue;
        }
#ifdef DEBUG_ON
        std::cout << "[acceptLoop] Authentication successful" << std::endl;
#endif

               // RECEIVE_CLIENT_INFO:
        char client_name[50] = {0};
        int name_len = recv(client_socket, client_name, sizeof(client_name) - 1, 0);
        if (name_len <= 0) {
            std::cerr << "failed to receive client name" << std::endl;
            closesocket(client_socket);
#ifdef DEBUG_ON
            std::cerr << "[acceptLoop] Failed to receive client name" << std::endl;
#endif
            continue;
        }
        client_name[name_len] = '\0';
#ifdef DEBUG_ON
        std::cout << "[acceptLoop] Received client name: " << client_name << std::endl;
#endif

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
#ifdef DEBUG_ON
        std::cout << "[acceptLoop] Client added, client thread detached" << std::endl;
#endif
    }
#ifdef DEBUG_ON
    std::cout << "[acceptLoop] Completed" << std::endl;
#endif
}

void Server::handleClient(int client_socket) {
#ifdef DEBUG_ON
    std::cout << "[handleClient] Invoked for socket: " << client_socket << std::endl;
#endif
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

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.erase(std::remove_if(clients.begin(), clients.end(),
                                     [client_socket](const ClientInfo& ci) { return ci.socfd == client_socket; }),
                      clients.end());
    }
#ifdef DEBUG_ON
    std::cout << "[handleClient] Client disconnected, removed from list" << std::endl;
#endif

    std::cout << "Client Handler Thread Terminated!! socket id = " << client_socket << std::endl;
#ifdef DEBUG_ON
    std::cout << "[handleClient] Completed for socket: " << client_socket << std::endl;
#endif
}

void Server::printClientsLoop(int intervalSeconds) {
#ifdef DEBUG_ON
    std::cout << "[printClientsLoop] Invoked" << std::endl;
#endif
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
#ifdef DEBUG_ON
    std::cout << "[printClientsLoop] Completed" << std::endl;
#endif
}

std::string Server::fetchLocalIP() {
#ifdef DEBUG_ON
    std::cout << "[fetchLocalIP] Invoked" << std::endl;
#endif
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
#ifdef DEBUG_ON
    std::cout << "[fetchLocalIP] Completed" << std::endl;
#endif
    return localIP;
}

// send to specific client
bool Server::sendFileToClient(int client_sock, const std::string path, const std::string msg) {
#ifdef DEBUG_ON
    std::cout << "[sendFileToClient] Invoked for socket: " << client_sock << ", path: " << path << std::endl;
#endif
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
    bool send_result = meta.send_on_socket(client_sock);
#ifdef DEBUG_ON
    std::cout << "[sendFileToClient] Completed for socket: " << client_sock << ", path: " << path << ", result: " << send_result << std::endl;
#endif
    return send_result;
}

// Broadcast to all
bool Server::sendFileToAllClients(const FileMeta& meta) {
#ifdef DEBUG_ON
    std::cout << "[sendFileToAllClients] Invoked" << std::endl;
#endif
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
#ifdef DEBUG_ON
    std::cout << "[sendFileToAllClients] Completed, all_ok: " << all_ok << std::endl;
#endif
    return all_ok;
}

// Receive from a client
FileMeta Server::receiveFileFromClient(int client_sock) {
#ifdef DEBUG_ON
    std::cout << "[receiveFileFromClient] Invoked for socket: " << client_sock << std::endl;
#endif
    FileMeta fm = FileMeta::recv_from_socket(client_sock);
#ifdef DEBUG_ON
    std::cout << "[receiveFileFromClient] Completed for socket: " << client_sock << std::endl;
#endif
    return fm;
}

#endif
