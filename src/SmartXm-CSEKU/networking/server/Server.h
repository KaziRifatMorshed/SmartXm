#ifndef SERVER_H
#define SERVER_H
#pragma once

#ifdef __linux__
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <string>
#include <mutex>
#include <sys/types.h>
#include <thread>
#include <vector>
#include "ClientInfo.h"
#include <networking/FileMeta.h>

class Server {
public:
    static Server* createServer(); // creator thake public e
    static bool isRunning() {return running;};
    std::string getStatus();
    std::string getLocalIP();
    bool sendFileToClient(int client_sock, std::string path, std::string msg); // bad, need FileMeta
    bool sendFileToAllClients(const FileMeta& meta); // build successful
    FileMeta receiveFileFromClient(int client_sock);
    void printClientsLoop(int intervalSeconds = 5);
    void stop();
    static std::string fetchLocalIP();

protected:
    Server(int port = 8080, const std::string& secret = "MySuperSecret123x"); // constructor protected e
    ~Server();
    static bool running; // serverExists
    std::string status;

private:
    int port;
    int server_fd;
    std::string secretKey;
    std::string localIP;
    static Server *serverInstance; // instance private e thake

    int start();

    void acceptLoop();
    void handleClient(int client_socket);

    std::vector<ClientInfo> clients;
    std::vector<ClientInfo> getClients();
    std::mutex clientsMutex;
    std::thread acceptThread;
    std::thread printerThread;
};

#endif // SERVER_H
