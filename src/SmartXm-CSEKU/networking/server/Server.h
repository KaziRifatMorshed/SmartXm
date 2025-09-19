#ifndef SERVER_H
#define SERVER_H
#pragma once
#include <algorithm>
#include <arpa/inet.h>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <ifaddrs.h>
#include <iostream>
#include <mutex>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include "ClientInfo.h"
#include "Msg.h"

class Server {
public:
    static Server* createServer();
    static int start();
    static void stop();
    static std::vector<ClientInfo> getClients();
    static std::string getStatus();
    static std::string getLocalIP();
    static bool sendFileToAllClients(std::string path);

    // For demo: Print clients every interval seconds
    static void printClientsLoop(int intervalSeconds = 5);

protected:
    Server(int port = 8080, const std::string& secret = "MySuperSecret123x");
    ~Server();
    static Server *serverInstance;

private:
    void acceptLoop();
    void handleClient(int client_socket);
    std::string fetchLocalIP();

    int port;
    std::string secretKey;
    std::string status;
    std::string localIP;

    int server_fd;
    static bool running; // serverExists
    std::vector<ClientInfo> clients;
    std::mutex clientsMutex;
    std::thread acceptThread;
    std::thread printerThread;
};

#endif // SERVER_H
