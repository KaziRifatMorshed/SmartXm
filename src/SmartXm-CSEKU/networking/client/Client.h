#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "Msg.h"
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <networking/FileMeta.h>

#define CLIENT_PORT 8080
#define CLIENT_BUFFER_SIZE 1024
#define CLIENT_SECRET_KEY "MySuperSecret123x"

class Client {
public:
    static Client* clientInstance; // pointer to instance
    static Client* createClient() { // static method accessing static instance variable
        if(clientInstance == nullptr){
            clientInstance = new Client();
        }
        return clientInstance;
    }

    Client(const Client&) = delete; // i do not understand
    Client& operator=(const Client&) = delete; // i do not understand
    Client(Client&&) = delete; // i do not understand
    Client& operator=(Client&&) = delete; // i do not understand

    bool connectToServer(const std::string& ip_addr = "127.0.0.1", int port = CLIENT_PORT);

    void chatLoop();

    void disconnect();
    static std::time_t lastLoginTime;
    void sendSubmission();
    void updateAccountInfo();
    static std::time_t getCurrentTime(){return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());};
    static std::string getCurrentTimeString(std::time_t t){
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&t), "%d-%m-%Y %H:%M:%S");
        return oss.str();
    };
    bool sendLoginInfoToServer();
    void storeLoginInfoToCache(); // client will be login for 3 hours; exceeding 3 hour will clear cache
    bool checkLoginInfoInCache(); //  each time GUI is opened, login info cache (stored in secondary memory) will be checked with lastLoginTime
    void getLeaderboardDataFromServer(); // data type may be changed later

    // FileMeta receive_file_from_server(int sock_fd);
    // bool send_file_to_server(int sock_fd, const std::string& path, const std::string& msg);

protected:
    Client();
    ~Client();


private:
    void receiveFileLoop();

    int sock_fd;
    std::string clientName;
    std::thread fileReceiverThread;
    bool connected;
    std::mutex connMutex;
};

#endif // CLIENT_H
