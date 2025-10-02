#ifdef __linux__

#include "./../server/Server.h"
#include "Client.h"
#include <filesystem>
#include <QMessageBox>
#include <networking/FileMeta.h>
#include <iomanip>
#include "dependencies/TarHandler/tarhandler.h"
#include <studentmodulev2.h>

Client *Client::clientInstance = nullptr;
std::time_t Client::lastLoginTime = 0;

Client::Client() : sock_fd(-1), connected(false) {
#ifdef DEBUG_ON
    std::cout << "[Client] Client object created." << std::endl;
#endif
}

Client::~Client() {
#ifdef DEBUG_ON
    std::cout << "[Client] Client object destroyed." << std::endl;
#endif
    disconnect();
}

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
        ::close(sock_fd);
        sock_fd = -1;
#ifdef DEBUG_ON
        std::cout << "[Client::disconnect] Disconnected successfully." << std::endl;
#endif
    }
}

void receive_file(int client_sock_fd) { // UNNECESSARY, ei kaj file receive loop e kore dicche 
    // receive file and check, ki aslo
}


extern StudentModuleV2 *studentModuleV2Pointer; // Declare somewhere accessible

void file_receive_loop(int sock_fd) {
    try {
        while (true) {
#ifdef DEBUG_ON
            std::cout << "[FileReceiver] Waiting for file/message from server..." << std::endl;
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
                                "rulebookArrived", // signal, not slot!
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

                if (meta.message == "questions.tar") {
                    TarHandler::extractTar("examResources", "questions.tar");
#ifdef DEBUG_ON
                    std::cout << "[FileReceiver] Extracted questions.tar." << std::endl;
#endif
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[FileReceiver] Error or connection closed: " << e.what() << std::endl;
    }
}
// Send file to server
bool send_file_to_server(int sock_fd, const std::string& path, const std::string& msg) {
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

    std::string fname = std::filesystem::path(path).filename();
    std::string ext = std::filesystem::path(path).extension().string();
    if (ext.size() && ext[0] == '.') ext = ext.substr(1);

    FileMeta meta(fname, ext, std::time(nullptr), std::move(filedata), msg);
#ifdef DEBUG_ON
    std::cout << "[send_file_to_server] Created FileMeta object." << std::endl;
#endif
    return meta.send_on_socket(sock_fd);
}

// Receive file from server
FileMeta receive_file_from_server(int sock_fd) {
#ifdef DEBUG_ON
    std::cout << "[receive_file_from_server] Receiving a file from server" << std::endl;
#endif
    return FileMeta::recv_from_socket(sock_fd);
}


void Client::updateAccountInfo() { // kivabe implement korbo ???
    if (!connected)
        return;
    std::string msg = "UPDATE_ACCOUNT";
    send(sock_fd, msg.c_str(), msg.size(), 0);
    // Add implementation for sending updated info as needed
}

bool Client::sendLoginInfoToServer() {
    if (!connected)
        return false;
    std::string email, password;

    std::string login_data = "LOGIN:" + email + ":" + password; // vulnerable
    send(sock_fd, login_data.c_str(), login_data.size(), 0);
#ifdef DEBUG_ON
    std::cout << "[Client::sendLoginInfoToServer] Login data sent." << std::endl;
#endif

    char buffer[CLIENT_BUFFER_SIZE]{};
    ssize_t valread = recv(sock_fd, buffer, CLIENT_BUFFER_SIZE, 0);
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
    send(sock_fd, msg.c_str(), msg.size(), 0);
#ifdef DEBUG_ON
    std::cout << "[Client::getLeaderboardDataFromServer] Sent request for leaderboard data." << std::endl;
#endif
    char buffer[CLIENT_BUFFER_SIZE]{};
    ssize_t valread = recv(sock_fd, buffer, CLIENT_BUFFER_SIZE, 0);
    if (valread > 0) {
        std::cout << "Leaderboard:\n" << std::string(buffer, valread) << "\n";
    }
}

bool Client::connectToServer(const std::string &ip_addr, int port) {
    std::lock_guard<std::mutex> lock(connMutex);

    if (connected)
        return true;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error creating socket\n";
        return false;
    }
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] Socket created." << std::endl;
#endif

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_addr.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported\n";
        ::close(sock_fd);
        return false;
    }
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] Address configured." << std::endl;
#endif

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
        std::cerr << "Connection Failed" << std::endl;
        ::close(sock_fd);
        return false;
    } else {
        std::cout << "Connected to server at: " << ip_addr << " : " << port
                  << std::endl;
    }
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] Connected to server." << std::endl;
#endif

           // Authenticate with secret key
           // connect houar por secret key poathiye check korbe je amar dol er lok ki na
    if (send(sock_fd, CLIENT_SECRET_KEY, strlen(CLIENT_SECRET_KEY), 0) < 0) {
        std::cerr << "Authentication failed" << std::endl;
    } else {
        std::cout << "Server Authentication Successful" << std::endl;
    }
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] Authentication sent." << std::endl;
#endif

    /* amar ager code e silo:
        // Send client name to server (ensure null-terminated)
        if (send(client_sock_fd, client_name, strlen(client_name), 0) < 0) {
            error("Client information sent failed");
        }
    */
    std::string clientIdentity = Server::fetchLocalIP();
    if (send(sock_fd, clientIdentity.c_str(), clientIdentity.length(), 0) < 0) {
        std::cerr << "Client info sent failed" << std::endl;
    }
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] Client identity sent." << std::endl;
#endif

           // char buffer[CLIENT_BUFFER_SIZE]{};
           // ssize_t valread = recv(sock_fd, buffer, CLIENT_BUFFER_SIZE, 0);
           // if (valread <= 0 || std::string(buffer) != "OK") {
           //     std::cerr << "Secret key mismatch or server error\n";
           //     ::close(sock_fd);
           //     return false;
           // }

    connected = true;
    // Start file receiving thread
    // fileReceiverThread = std::thread(&Client::receiveFileLoop, this);
    // fileReceiverThread = std::thread(&Client::receiveFileLoop, sock_fd); // not
    // working fileReceiverThread = std::thread(receiveFileLoop(), sock_fd);
    // fileReceiverThread.detach();

           // std::thread t(&Client::receiveFileLoop, this);
           // std::thread t(receive_file, sock_fd);
    std::thread t(file_receive_loop, sock_fd);
    t.detach();
#ifdef DEBUG_ON
    std::cout << "[Client::connectToServer] File receiver thread detached." << std::endl;
#endif

    return true;
}

#endif
