#ifdef _WIN32
#include <ShlObj.h> // Include for SHGetFolderPath

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
#include <iomanip>
#include <studentmodulev2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Shell32.lib") // Link with Shell32.lib for SHGetFolderPath

Client *Client::clientInstance = nullptr;
std::time_t Client::lastLoginTime = 0;

Client::Client() : sock_fd(INVALID_SOCKET), connected(false) {}

Client::~Client() { disconnect(); }



void Client::disconnect() {
    std::lock_guard<std::mutex> lock(connMutex);
    if (connected) {
#ifdef DEBUG_ON
        std::cout << "[Client::disconnect] Disconnecting from server." << std::endl;
#endif
        connected = false;
        if (fileReceiverThread.joinable()) {
#ifdef DEBUG_ON
            std::cout << "[Client::disconnect] Joining file receiver thread." << std::endl;
#endif
            fileReceiverThread.join();
        }
        closesocket(sock_fd);
        sock_fd = INVALID_SOCKET;
        WSACleanup();
#ifdef DEBUG_ON
        std::cout << "[Client::disconnect] Disconnected successfully." << std::endl;
#endif
    }
}



// Receive file from server
FileMeta receive_file_from_server(SOCKET sock_fd) {
#ifdef DEBUG_ON
    std::cout << "[receive_file_from_server] Received a file from server" << std::endl;
#endif
    return FileMeta::recv_from_socket(sock_fd);
}



// Send file to server
bool send_file_to_server(SOCKET sock_fd, const std::string& path, const std::string& msg) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
#ifdef DEBUG_ON
        std::cerr << "[send_file_to_server] Failed to open file: " << path << std::endl;
#endif
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t sz = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> filedata(sz);
    file.read(filedata.data(), sz);
#ifdef DEBUG_ON
    std::cout << "[send_file_to_server] Read " << sz << " bytes from file." << std::endl;
#endif

    std::string fname = std::filesystem::path(path).filename().string();
    std::string ext = std::filesystem::path(path).extension().string();
    if (ext.size() && ext[0] == '.') ext = ext.substr(1);

    FileMeta meta(fname, ext, std::time(nullptr), std::move(filedata), msg);
#ifdef DEBUG_ON
    std::cout << "[send_file_to_server] Created FileMeta object." << std::endl;
#endif
    return meta.send_on_socket(sock_fd);
}

extern StudentModuleV2 *studentModuleV2Pointer; // Declare somewhere accessible

void file_receive_loop(SOCKET sock_fd) {
    try {
        while (true) {
#ifdef DEBUG_ON
            std::cout << "[FileReceiver][file_receive_loop] Waiting for file/message from server..." << std::endl;
#endif
            FileMeta meta = FileMeta::recv_from_socket(sock_fd);
#ifdef DEBUG_ON
            std::cout << "[FileReceiver] Received file: "
                      << meta.filename
                      << " (." << meta.extension << "), "
                      << "size: " << meta.file_data.size() << " bytes, "
                      << "sent at: " << std::put_time(std::localtime(&meta.sent_time), "%d-%m-%Y %H:%M:%S")
                      << std::endl;
            std::cout << "[FileReceiver] Server message: " << meta.message << std::endl;
#endif

                   // Use meta.message as the file type/purpose indicator
            std::string save_name;
            if (meta.message == "rulebook") {
                save_name = "./examResources/rulebook." + meta.extension;
#ifdef DEBUG_ON
                std::cout << "[FileReceiver] Saving as rulebook." << meta.extension << std::endl;
#endif

                if(studentModuleV2Pointer){
                    std::ofstream ofs(save_name, std::ios::binary);
                    if (ofs) {
                        ofs.write(meta.file_data.data(), meta.file_data.size());
                        ofs.close();
#ifdef DEBUG_ON
                        std::cout << "[FileReceiver] Rulebook written to file." << std::endl;
#endif
       // Notify UI (in main thread)
                        if (studentModuleV2Pointer)
                            QMetaObject::invokeMethod(
                                studentModuleV2Pointer,
                                "rulebookReceived",
                                Qt::QueuedConnection
                                );
#ifdef DEBUG_ON
                        std::cout << "[FileReceiver] rulebookArrived signal sent." << std::endl;
#endif
                    }
                }

            } else if (meta.message == "questions.tar") {
                save_name = "./examResources/questions." + meta.extension;
#ifdef DEBUG_ON
                std::cout << "[FileReceiver] Saving as questions.tar." << meta.extension << std::endl;
#endif
            } else if (meta.message == "extra") {
                save_name = "./examResources/notice." + meta.extension;
#ifdef DEBUG_ON
                std::cout << "[FileReceiver] Saving as notice." << meta.extension << std::endl;
#endif
            }
            // else { // kisu korbo na
            //     save_name = meta.filename; // fallback to original filename
            // }

            std::ofstream ofs(save_name, std::ios::binary);
            if (!ofs) {
                std::cerr << "[FileReceiver] Failed to write file: " << save_name << std::endl;
            } else {
                ofs.write(meta.file_data.data(), meta.file_data.size());
                ofs.close();
#ifdef DEBUG_ON
                std::cout << "[FileReceiver] File saved as: " << save_name << std::endl;
#endif

       // if (meta.message == "questions.tar") { // TarHandler class is not provided, thus commenting it out
       //     TarHandler::extractTar("examResources", "questions.tar");
#ifdef DEBUG_ON
                //     std::cout << "[FileReceiver] Extracted questions.tar." << std::endl;
#endif
       // }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[FileReceiver] Error or connection closed: " << e.what() << std::endl;
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
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] Socket created." << std::endl;
#endif

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (InetPtonA(AF_INET, ip_addr.c_str(), &server_addr.sin_addr) != 1) {
        std::cerr << "Invalid address/ Address not supported\n";
        closesocket(sock_fd);
        WSACleanup();
        return false;
    }
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] Address configured." << std::endl;
#endif

    if (connect(sock_fd, (SOCKADDR *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection Failed: " << WSAGetLastError() << std::endl;
        closesocket(sock_fd);
        WSACleanup();
        return false;
    } else {
#ifdef DEBUG_ON
        std::cout << "Connected to server at: " << ip_addr << " : " << port << std::endl;
#endif
    }
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] Connected to server." << std::endl;
#endif

           // Authenticate with secret key
    if (send(sock_fd, CLIENT_SECRET_KEY, (int)strlen(CLIENT_SECRET_KEY), 0) < 0) {
        std::cerr << "Authentication failed" << std::endl;
    } else {
#ifdef DEBUG_ON
        std::cout << "Server Authentication Successful" << std::endl;
#endif
    }
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] Authentication sent." << std::endl;
#endif

           // Send client name to server
    std::string clientIdentity = Server::fetchLocalIP();
    if (send(sock_fd, clientIdentity.c_str(), (int)clientIdentity.length(), 0) < 0) {
        std::cerr << "Client info sent failed" << std::endl;
    }
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] Client identity sent." << std::endl;
#endif

    connected = true;

    // Start file receiving thread (optional)
    std::thread t(file_receive_loop, sock_fd);
    t.detach();
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] File receiver thread detached." << std::endl;
#endif

    return true;
}

void Client::updateAccountInfo() { // kivabe implement korbo ???
    if (!connected)
        return;
    std::string msg = "UPDATE_ACCOUNT";
    send(sock_fd, msg.c_str(), (int)msg.size(), 0);
    // Add implementation for sending updated info as needed
}


bool Client::sendLoginInfoToServer() {
    if (!connected)
        return false;
    std::string email, password;

    std::string login_data = "LOGIN:" + email + ":" + password; // vulnerable
    send(sock_fd, login_data.c_str(), (int)login_data.size(), 0);
#ifdef DEBUG_ON
    std::cout << "[Client::sendLoginInfoToServer] Login data sent." << std::endl;
#endif

    char buffer[CLIENT_BUFFER_SIZE]{};
    int valread = recv(sock_fd, buffer, CLIENT_BUFFER_SIZE, 0);
    std::string response(buffer, valread);
    if (response == "LOGIN_SUCCESS") {
        clientName = email;
        lastLoginTime = getCurrentTime();
        storeLoginInfoToCache();
#ifdef DEBUG_ON
        std::cout << "[Client::sendLoginInfoToServer] Login successful!" << std::endl;
#endif
        return true;
    }
#ifdef DEBUG_ON
    std::cout << "[Client::sendLoginInfoToServer] Login failed." << std::endl;
#endif
    return false;
}

void Client::storeLoginInfoToCache() {
    std::ofstream ofs("login_cache.txt");
    ofs << clientName << "\n" << lastLoginTime << "\n";
    ofs.close();
#ifdef DEBUG_ON
    std::cout << "[Client::storeLoginInfoToCache] Login info stored to cache." << std::endl;
#endif
}

bool Client::checkLoginInfoInCache() { // CHECK LATER
    std::ifstream ifs("login_cache.txt");
    if (!ifs)
        return false;
    std::string cached_name;
    std::time_t cached_time;
    ifs >> cached_name >> cached_time;
    ifs.close();
    std::time_t now = getCurrentTime();
    if (cached_name == clientName && (now - cached_time) < 3 * 60 * 60) {
        lastLoginTime = cached_time;
#ifdef DEBUG_ON
        std::cout << "[Client::checkLoginInfoInCache] Login info valid in cache." << std::endl;
#endif
        return true;
    }
#ifdef DEBUG_ON
    std::cout << "[Client::checkLoginInfoInCache] Login info not valid in cache." << std::endl;
#endif
    return false;
}

void Client::getLeaderboardDataFromServer() { // FUTURE IMPLEMENTATION
    if (!connected)
        return;
    std::string msg = "GET_LEADERBOARD";
    send(sock_fd, msg.c_str(), (int)msg.size(), 0);
#ifdef DEBUG_ON
    std::cout << "[Client::getLeaderboardDataFromServer] Sent request for leaderboard data." << std::endl;
#endif
    char buffer[CLIENT_BUFFER_SIZE]{};
    int valread = recv(sock_fd, buffer, CLIENT_BUFFER_SIZE, 0);
    if (valread > 0) {
        std::cout << "Leaderboard:\n" << std::string(buffer, valread) << "\n";
    }
}

#endif
