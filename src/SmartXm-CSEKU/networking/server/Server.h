#ifndef SERVER_H
#define SERVER_H
#pragma once
#include <arpa/inet.h>
#include <ifaddrs.h>
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

class Server {
public:
    static Server* createServer(); // creator thake public e
    static bool isRunning() {return running;};
    std::string getStatus();
    std::string getLocalIP();
    bool sendFileToAllClients(std::string path);
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
