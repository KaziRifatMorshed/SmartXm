#ifdef __linux__

#include "./../server/Server.h"
#include "Client.h"
#include <filesystem>
#include <QMessageBox>
#include <studentmodule.cpp>
#include <networking/FileMeta.h>
#include <iomanip>
#include "dependencies/TarHandler/tarhandler.h"

Client *Client::clientInstance = nullptr;
std::time_t Client::lastLoginTime = 0;

Client::Client() : sock_fd(-1), connected(false) {}

Client::~Client() { disconnect(); }

void Client::disconnect() {
  std::lock_guard<std::mutex> lock(connMutex);
  if (connected) {
    connected = false;
    if (fileReceiverThread.joinable()) {
      fileReceiverThread.join();
    }
    ::close(sock_fd);
    sock_fd = -1;
  }
}

/*
void Client::chatLoop() {
  while (connected) {
    std::string msg;
    std::getline(std::cin, msg);
    if (msg == "/quit")
      break;
    send(sock_fd, msg.c_str(), msg.size(), 0);
  }
  disconnect();
}
*/

void receive_file(int client_sock_fd) {
    // receive file and check, ki aslo
}

/*
void receive_file(int client_sock_fd) {
  while (true) {
    size_t file_size;
    size_t bytes_received =
        recv(client_sock_fd, &file_size, sizeof(file_size), MSG_WAITALL);
    if (bytes_received <= 0) {
      std::cerr << "Server disconnected or error receiving file size."
                << std::endl;
      break;
    }

    std::cout << "Receiving file of size: " << file_size << " bytes."
              << std::endl;

    char *file_buffer = new char[file_size];
    bytes_received = recv(client_sock_fd, file_buffer, file_size, MSG_WAITALL);
    if (bytes_received != file_size) {
      std::cerr << "Error receiving file data. Expected: " << file_size
                << ", received: " << bytes_received << std::endl;
      delete[] file_buffer;
      break;
    }

    std::string filename = "rulebook.pdf";
    std::ofstream output_file(filename, std::ios::binary);
    if (!output_file.is_open()) {
      std::cerr << "Error opening file for writing: " << filename << std::endl;
      delete[] file_buffer;
      break;
    }


    output_file.write(file_buffer, file_size);
    output_file.close();
    std::cout << "File saved as: " << filename << std::endl;

    delete[] file_buffer;
  }
}
*/

/*
void Client::receiveFileLoop() { // NOT USING ANYMORE !!!!!!!!
  // Event-driven: wait for "file" message from server, then receive file
  while (connected) {
    char buffer[CLIENT_BUFFER_SIZE]{};
    ssize_t valread = recv(sock_fd, buffer, CLIENT_BUFFER_SIZE, 0);
    if (valread <= 0)
      break;

    std::string msg(buffer, valread);

    if (msg == "RULEBOOK_TRANSFER") {
      // Receive file size
      ssize_t sz = recv(sock_fd, buffer, CLIENT_BUFFER_SIZE, 0);
      size_t file_size = std::stoul(std::string(buffer, sz));
      std::ofstream outfile("rulebook.pdf", std::ios::binary);
      size_t total_received = 0;
      while (total_received < file_size) {
        ssize_t n = recv(sock_fd, buffer, CLIENT_BUFFER_SIZE, 0);
        if (n <= 0)
          break;
        outfile.write(buffer, n);
        total_received += n;
      }
      outfile.close();
      std::cout << "Rulebook received!\n";
    }
    // Add more events as needed
  }
}
*/

// new
extern StudentModule *studentModulePointer; // Declare somewhere accessible

void file_receive_loop(int sock_fd) {
    try {
        while (true) {
            std::cout << "[FileReceiver] Waiting for file/message from server..." << std::endl;
            FileMeta meta = FileMeta::recv_from_socket(sock_fd);

            std::cout << "[FileReceiver] Received file: "
                      << meta.filename
                      << " (." << meta.extension << "), "
                      << "size: " << meta.file_data.size() << " bytes, "
                      << "sent at: " << std::put_time(std::localtime(&meta.sent_time), "%d-%m-%Y %H:%M:%S")
                      << std::endl;
            std::cout << "[FileReceiver] Server message: " << meta.message << std::endl;

            // Use meta.message as the file type/purpose indicator
            std::string save_name;
            if (meta.message == "rulebook") {
                save_name = "./examResources/rulebook." + meta.extension;

                if(studentModulePointer){
                    std::ofstream ofs(save_name, std::ios::binary);
                    if (ofs) {
                        ofs.write(meta.file_data.data(), meta.file_data.size());
                        ofs.close();
                        // Notify UI (in main thread)
                        if (studentModulePointer)
                            QMetaObject::invokeMethod(
                                studentModulePointer,
                                "rulebookArrived", // signal, not slot!
                                Qt::QueuedConnection
                                );
                    }
                }

            } else if (meta.message == "questions.tar") {
                save_name = "./examResources/questions." + meta.extension;
            } else if (meta.message == "extra") {
                save_name = "./examResources/notice." + meta.extension;
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
                std::cout << "[FileReceiver] File saved as: " << save_name << std::endl;

                if (meta.message == "questions.tar") {
                    TarHandler::extractTar("examResources", "questions.tar");
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
    if (!file.is_open()) return false;
    file.seekg(0, std::ios::end);
    size_t sz = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> filedata(sz);
    file.read(filedata.data(), sz);

    std::string fname = std::filesystem::path(path).filename();
    std::string ext = std::filesystem::path(path).extension().string();
    if (ext.size() && ext[0] == '.') ext = ext.substr(1);

    FileMeta meta(fname, ext, std::time(nullptr), std::move(filedata), msg);
    return meta.send_on_socket(sock_fd);
}

// Receive file from server
FileMeta receive_file_from_server(int sock_fd) {
    std::cout << "received a file from server" << std::endl;
    return FileMeta::recv_from_socket(sock_fd);
}


/*
void Client::sendSubmission() { // need to check later
  if (!connected)
    return;
  std::string submissionPath;
  std::cout << "Enter submission file path: ";
  std::getline(std::cin, submissionPath);

  if (!std::filesystem::exists(submissionPath)) {
    std::cerr << "Submission file not found.\n";
    return;
  }

  std::ifstream infile(submissionPath, std::ios::binary | std::ios::ate);
  auto file_size = infile.tellg();
  infile.seekg(0);

  // Notify server
  std::string header = "SUBMISSION_TRANSFER";
  send(sock_fd, header.c_str(), header.size(), 0);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::string size_str = std::to_string(file_size);
  send(sock_fd, size_str.c_str(), size_str.size(), 0);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  char buffer[CLIENT_BUFFER_SIZE]{};
  while (!infile.eof()) {
    infile.read(buffer, CLIENT_BUFFER_SIZE);
    std::streamsize n = infile.gcount();
    send(sock_fd, buffer, n, 0);
  }
  infile.close();
  std::cout << "Submission sent!\n";
}
*/


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

  char buffer[CLIENT_BUFFER_SIZE]{};
  ssize_t valread = recv(sock_fd, buffer, CLIENT_BUFFER_SIZE, 0);
  std::string response(buffer, valread);
  if (response == "LOGIN_SUCCESS") {
    clientName = email;
    lastLoginTime = getCurrentTime();
    storeLoginInfoToCache();
    std::cout << "Login successful!\n";
    return true;
  }
  std::cout << "Login failed.\n";
  return false;
}

void Client::storeLoginInfoToCache() {
  std::ofstream ofs("login_cache.txt");
  ofs << clientName << "\n" << lastLoginTime << "\n";
  ofs.close();
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
    return true;
  }
  return false;
}

void Client::getLeaderboardDataFromServer() { // FUTURE IMPLEMENTATION
  if (!connected)
    return;
  std::string msg = "GET_LEADERBOARD";
  send(sock_fd, msg.c_str(), msg.size(), 0);
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

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  if (inet_pton(AF_INET, ip_addr.c_str(), &server_addr.sin_addr) <= 0) {
    std::cerr << "Invalid address/ Address not supported\n";
    ::close(sock_fd);
    return false;
  }

  if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    std::cerr << "Connection Failed" << std::endl;
    ::close(sock_fd);
    return false;
  } else {
    std::cout << "Connected to server at: " << ip_addr << " : " << port
              << std::endl;
  }

  // Authenticate with secret key
  // connect houar por secret key poathiye check korbe je amar dol er lok ki na
  if (send(sock_fd, CLIENT_SECRET_KEY, strlen(CLIENT_SECRET_KEY), 0) < 0) {
    std::cerr << "Authentication failed" << std::endl;
  } else {
    std::cout << "Server Authentication Successful" << std::endl;
  }

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

  return true;
}

#endif
