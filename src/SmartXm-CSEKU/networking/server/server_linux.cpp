#ifdef __linux__

#include "Server.h"
#include <fstream>
#include <sstream>

Server::Server(int port_, const std::string& secret)
    : port(port_), secretKey(secret), status("NOT RUNNING"), server_fd(-1)
{
    localIP = fetchLocalIP();
    Server::start();
}

Server::~Server() {
    stop();
}

Server* Server::createServer(){ // Singleton approach
    if(!running){
        Server::serverInstance = new Server();
        running = true;
    }
    return Server::serverInstance;
}


int Server::start() {
    if (running) return 0;
    int opt = 1;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        status = "FAILED";
        return 1;
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        status = "FAILED";
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        status = "FAILED";
        return 1;
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        status = "FAILED";
        return 1;
    }

    status = "RUNNING";
    running = true;

    std::cout << "Welcome to SmartXm SERVER" << std::endl;
    std::cout << "Server will listen on port " << port << std::endl;
    std::cout << "Server local IP (give this to client): " << localIP << std::endl;
    std::cout << "Waiting for client connections..." << std::endl;

    // Start threads
    acceptThread = std::thread(&Server::acceptLoop, this);
    printerThread = std::thread(&Server::printClientsLoop, this, 5);

    return 0;
}

void Server::stop() {
    if (!running) return;
    running = false;
    status = "STOPPED";
    std::cout << "STOPPING SERVER" << std::endl;

    // Close the listening socket to unblock accept()
    close(server_fd);

    // Close all client sockets
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (auto& ci : clients) {
            close(ci.socfd);
        }
        clients.clear();
    }

    // Join threads
    if (acceptThread.joinable()) acceptThread.join();
    if (printerThread.joinable()) printerThread.join();

    std::cout << "SERVER SHOULD BE STOPPED" << std::endl;
    return;
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
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (!running) break;
        if (client_socket < 0) {
            if (running) perror("failed to accept connection");
            continue;
        }

        // AUTHENTICATION:
        char auth_buffer[256] = {0};
        ssize_t received_length = recv(client_socket, auth_buffer, sizeof(auth_buffer) - 1, 0);
        if (received_length <= 0) {
            perror("failed to receive secret key");
            close(client_socket);
            continue;
        }
        auth_buffer[received_length] = '\0';
        if (secretKey != std::string(auth_buffer)) {
            std::cout << secretKey << " " << std::string(auth_buffer) << "\n";
            std::cout << "Authentication failed for client from " << inet_ntoa(client_addr.sin_addr) << std::endl;
            close(client_socket);
            continue;
        }

        // RECEIVE_CLIENT_INFO:
        char client_name[50] = {0};
        ssize_t name_len = recv(client_socket, client_name, sizeof(client_name) - 1, 0);
        if (name_len <= 0) {
            perror("failed to receive client name");
            close(client_socket);
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
    Msg msg;
    while (running) {
        ssize_t valread = recv(client_socket, &msg, sizeof(msg), 0);
        if (valread <= 0) {
            std::cout << "Client (socket_id=" << client_socket << ") disconnected!" << std::endl;
            break;
        }
        else {
            std::cout << "Received msg from '" << msg.sender_name << "': " << msg.text << std::endl;
        }
    }
    close(client_socket);
    // Remove client from list
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.erase(std::remove_if(clients.begin(), clients.end(),
                                     [client_socket](const ClientInfo& ci) { return ci.socfd == client_socket; }),
                      clients.end());
    }
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
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in* sa;
    char* addr;
    std::string found = "error";

    if (getifaddrs(&ifap) == -1) {
        perror("getifaddrs");
        return found;
    }

    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            sa = (struct sockaddr_in*)ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            if (strcmp(ifa->ifa_name, "lo") != 0 && strcmp(addr, "127.0.0.1") != 0) {
                found = std::string(addr);
                break;
            }
        }
    }
    freeifaddrs(ifap);
    return found;
}

bool Server::sendFileToAllClients(std::string path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContent = buffer.str();
    size_t fileSize = fileContent.size();

    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto& client : clients) {
        int client_socket = client.socfd;

        // 1. Send file size
        ssize_t bytesSent = send(client_socket, &fileSize, sizeof(fileSize), 0);
        if (bytesSent != sizeof(fileSize)) {
            std::cerr << "Failed to send file size to client " << client.clientName << std::endl;
            continue;
        }

        // 2. Send file content
        bytesSent = send(client_socket, fileContent.c_str(), fileSize, 0);
        if (bytesSent != fileSize) {
            std::cerr << "Failed to send file content to client " << client.clientName << std::endl;
            continue;
        }

        std::cout << "File '" << path << "' sent to client " << client.clientName << std::endl;
    }

    return true;
}


#endif
