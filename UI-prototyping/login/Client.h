#ifndef CLIENT_LINUX_H
#define CLIENT_LINUX_H
#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "Msg.h"

class Client {
public:
    Client(int port = 8080, const std::string& secret = "MySuperSecret123x");
    ~Client();

    int start(const std::string& server_ip = "127.0.0.1"); // Connect to server
    void stop();
    std::string getStatus() const;
    std::string getLocalIP() const;

    int sendMessage(const Msg& msg);
    int sendAuthentication();
    int sendClientName(const std::string& name);

private:
    int port;
    std::string secretKey;
    std::string status;
    std::string localIP;

    int client_fd;
    bool running;

    std::string fetchLocalIP();
};

#endif // CLIENT_LINUX_H
