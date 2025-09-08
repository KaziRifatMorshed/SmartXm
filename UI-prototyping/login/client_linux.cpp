#ifndef LINUX_CLIENT
#define LINUX_CLIENT
#include "Client.h"
#include <iostream>
#include <limits>
#include <cstring>

Client::Client(int port_, const std::string& secret)
: port(port_), secretKey(secret), status("NOT CONNECTED"), client_fd(-1), running(false) {
    localIP = fetchLocalIP();
}

Client::~Client() {
    stop();
}

int Client::start(const std::string& server_ip) {
    if (running) return 0;

    struct sockaddr_in server_addr;
    char client_name[50] = {0};

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        status = "FAILED";
        return 1;
    }

    // Prepare server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        status = "FAILED";
        return 1;
    }

    // Connect to server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
        status = "FAILED";
        return 1;
    }

    running = true;
    status = "CONNECTED";
    std::cout << "Connected to server at: " << server_ip << " : " << port << std::endl;

    // Authentication
    if (sendAuthentication() < 0) {
        perror("Authentication failed");
        status = "AUTH_FAILED";
        stop();
        return 1;
    }

    // Get client name
    std::cout << "Enter your name: ";
    std::cin.getline(client_name, sizeof(client_name));
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (sendClientName(std::string(client_name)) < 0) {
        perror("Sending client name failed");
        status = "FAILED";
        stop();
        return 1;
    }

    std::cout << "\nNow, you should be able to chat with the server..." << std::endl;

    while (running) {
        std::cout << "Write Message (and hit Enter): ";
        char temp[BUFFER_SIZE] = {0};
        std::cin.getline(temp, BUFFER_SIZE);

        if (std::cin.fail()) {
            std::cout << "INPUT IS TOO LONG !!! Only first " << (BUFFER_SIZE - 1) << " characters will be used." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (strcmp(temp, "\\c") == 0) {
            std::cout << "Closing chatting..." << std::endl;
            break;
        }

        Msg msg;
        strncpy(msg.sender_name, client_name, sizeof(msg.sender_name) - 1);
        msg.sender_name[sizeof(msg.sender_name) - 1] = '\0';
        strncpy(msg.text, temp, sizeof(msg.text) - 1);
        msg.text[sizeof(msg.text) - 1] = '\0';

        if (sendMessage(msg) < 0) {
            perror("Message send failed");
            break;
        } else {
            std::cout << "ACK: Msg sent to server : " << temp << std::endl;
        }
    }

    stop();
    std::cout << "Client has been closed" << std::endl;
    return 0;
}

void Client::stop() {
    if (!running) return;

    running = false;
    status = "DISCONNECTED";

    if (client_fd != -1) {
        close(client_fd);
        client_fd = -1;
    }
}

std::string Client::getStatus() const {
    return status;
}

std::string Client::getLocalIP() const {
    return localIP;
}

int Client::sendAuthentication() {
    if (send(client_fd, secretKey.c_str(), secretKey.size(), 0) < 0) {
        return -1;
    }
    return 0;
}

int Client::sendClientName(const std::string& name) {
    if (send(client_fd, name.c_str(), name.size(), 0) < 0) {
        return -1;
    }
    return 0;
}

int Client::sendMessage(const Msg& msg) {
    if (send(client_fd, &msg, sizeof(msg), 0) < 0) {
        return -1;
    }
    return 0;
}

std::string Client::fetchLocalIP() {
    return "127.0.0.1";
}

void Client::setClientName(std::string name){
    Client::clientName = name;
}

std::string Client::getClientName(){
    return clientName;
}

#endif
