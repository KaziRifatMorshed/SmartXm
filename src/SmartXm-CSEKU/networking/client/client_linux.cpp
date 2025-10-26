#ifdef __linux__

#include "./../server/Server.h"
#include "Client.h"
#include "dependencies/TarHandler/tarhandler.h"
#include <QMessageBox>
#include <filesystem>
#include <iomanip>
#include <networking/FileMeta.h>
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
      std::cout << "[Client::disconnect] Joining file receiver thread."
                << std::endl;
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

// void receive_file(int client_sock_fd) { // UNNECESSARY, ei kaj file receive
// loop e kore dicche
//     // receive file and check, ki aslo
// }

extern StudentModuleV2 *studentModuleV2Pointer; // Declare somewhere accessible

void file_receive_loop(int sock_fd) {
  try {
    while (true) {
#ifdef DEBUG_ON
      std::cout << "[FileReceiver] Waiting for file/message from server..."
                << std::endl;
#endif

      FileMeta meta = FileMeta::recv_from_socket(sock_fd);

#ifdef DEBUG_ON
      std::cout << "[FileReceiver] Received file: " << meta.filename << " (."
                << meta.extension << "), "
                << "size: " << meta.file_data.size() << " bytes, "
                << "sent at: "
                << std::put_time(std::localtime(&meta.sent_time),
                                 "%d-%m-%Y %H:%M:%S")
                << std::endl;
      std::cout << "[FileReceiver] Server message: " << meta.message
                << std::endl;
#endif

      // Use meta.title as the file type/purpose indicator
      std::string save_name;

      /* ------------ RULEBOOK ------------ */

      if (meta.title == "rulebook") {
        save_name = "./examResources/rulebook." + meta.extension;
#ifdef DEBUG_ON
        std::cout << "[FileReceiver] Saving as rulebook." << meta.extension
                  << std::endl;
#endif

        if (studentModuleV2Pointer) {
          std::ofstream ofs(save_name, std::ios::binary);
          if (ofs) {
            ofs.write(meta.file_data.data(), meta.file_data.size());
            ofs.close();
#ifdef DEBUG_ON
            std::cout << "[FileReceiver] Rulebook written to file."
                      << std::endl;
#endif
            // Notify UI (in main thread)
            if (studentModuleV2Pointer)
              QMetaObject::invokeMethod(studentModuleV2Pointer,
                                        "rulebookArrived", // signal, not slot!
                                        Qt::QueuedConnection);
#ifdef DEBUG_ON
            std::cout << "[FileReceiver] rulebookArrived signal sent."
                      << std::endl;
#endif
          }
        }
      }

      /* ------------ HELLO ------------ */

      if (meta.title == "HELLO") {
        save_name = "./cache/helloFromServer." + meta.extension;
#ifdef DEBUG_ON
        std::cout << "[FileReceiver] Saving as Hello." << meta.extension
                  << " for TESTING" << std::endl;
#endif

        if (studentModuleV2Pointer) {
          std::ofstream ofs(save_name, std::ios::binary);
          if (ofs) {
            ofs.write(meta.file_data.data(), meta.file_data.size());
            ofs.close();
#ifdef DEBUG_ON
            std::cout
                << "[FileReceiver] Hello file (FOR TESTING) written to file."
                << std::endl;
#endif
            // Notify UI (in main thread)
            //             if (studentModuleV2Pointer)
            //               QMetaObject::invokeMethod(studentModuleV2Pointer,
            //                                         "HelloFileArrived", //
            //                                         signal, not slot! // DOES
            //                                         NOT EXISTS CURRENTLY
            //                                         Qt::QueuedConnection);
            // #ifdef DEBUG_ON
            //             std::cout << "[FileReceiver] HelloFileArrived signal
            //             sent."
            //                       << std::endl;
            // #endif
          }
        }
      }

      /* ------------ QUESTIONS ------------ */

      else if (meta.title == "QUESTIONS") {
        std::string temp_save =
            "/home/seam/Desktop/SmartXm/src/SmartXm-CSEKU/examResources/";
        save_name = temp_save + "questions." + meta.extension;
        // save_name = "./examResources/questions." + meta.extension;
#ifdef DEBUG_ON
        std::cout << "[FileReceiver] Saving as questions.tar." << meta.extension
                  << std::endl;
#endif
      }

      /* ------------ EXTRA ------------ */

      else if (meta.title == "extra") {
        save_name = "./examResources/notice." + meta.extension;
#ifdef DEBUG_ON
        std::cout << "[FileReceiver] Saving as notice." << meta.extension
                  << std::endl;
#endif
      }
      // else { // kisu korbo na
      //     save_name = meta.filename; // fallback to original filename
      // }

      /* ------------ FILE SAVE LOGIC ------------ */

      std::ofstream ofs(save_name, std::ios::binary);
      if (!ofs) {
        std::cerr << "[FileReceiver] Failed to write file: " << save_name
                  << std::endl;
      } else {
        ofs.write(meta.file_data.data(), meta.file_data.size());
        ofs.close();
#ifdef DEBUG_ON
        std::cout << "[FileReceiver] File saved as: " << save_name << std::endl;
#endif

        if (meta.title == "QUESTIONS") {
          TarHandler::extractTar("examResources", "questions.tar");
#ifdef DEBUG_ON
          std::cout << "[FileReceiver] Extracted questions.tar." << std::endl;
#endif
        }
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "[FileReceiver] Error or connection closed: " << e.what()
              << std::endl;
  }
}

// Send file to server
bool Client::send_file_to_server(const std::string &path,
                                 const std::string &title,
                                 const std::string &msg) {
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open()) {
#ifdef DEBUG_ON
    std::cerr << "[send_file_to_server] Failed to open file: " << path
              << std::endl;
#endif
    return false;
  }
  file.seekg(0, std::ios::end);
  size_t sz = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<char> filedata(sz);
  file.read(filedata.data(), sz);
#ifdef DEBUG_ON
  std::cout << "[send_file_to_server] Read " << sz << " bytes from file."
            << std::endl;
#endif

  std::string fname = std::filesystem::path(path).filename();
  std::string ext = std::filesystem::path(path).extension().string();
  if (ext.size() && ext[0] == '.')
    ext = ext.substr(1);

  FileMeta meta(title, fname, ext, std::time(nullptr), std::move(filedata),
                msg);
#ifdef DEBUG_ON
  std::cout << "[send_file_to_server] Created FileMeta object." << std::endl;
#endif
  return meta.send_on_socket(sock_fd);
}




// Receive file from server
FileMeta Client::receive_file_from_server() {
#ifdef DEBUG_ON
  std::cout << "[receive_file_from_server] Receiving a file from server"
            << std::endl;
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




QString Client::sendLoginInfoToServer(std::string email, std::string password) {
  if (!connected)
        return nullptr;
  std::string login_data = email + ":" + password; //  should encrypt this!

  // Create an empty FileMeta object to act as a command packet
  FileMeta meta;
  meta.title = "LOGIN_REQ";
  meta.message = login_data;
  meta.sent_time = std::time(nullptr);

  if (!meta.send_on_socket(sock_fd)) {
    std::cerr << "[Client] Failed to send LOGIN request." << std::endl;
    return nullptr;
  }
#ifdef DEBUG_ON
  std::cout << "[Client::sendLoginInfoToServer] Login data sent via FileMeta."
            << std::endl;
#endif

  // Call the static function and assign its return value
  FileMeta response = FileMeta::recv_from_socket(sock_fd);

  if (response.title == "LS") {
    clientName = email;
    lastLoginTime = getCurrentTime();
    std::cout << "[Client::sendLoginInfoToServer] Login successful!"
              << std::endl;
    return QString::fromStdString(response.message);
  } else if (response.title == "LF") {
    std::cout << "[Client::sendLoginInfoToServer] Login failed." << std::endl;
    return nullptr;
  }

  // Add a fallback for unexpected responses
  std::cerr
      << "[Client::sendLoginInfoToServer] Received unknown login response: "
      << response.title << std::endl;
  return nullptr;
}




void Client::getLeaderboardDataFromServer() {
  if (!connected)
    return;

  FileMeta meta;
  meta.title = "GET_LEADERBOARD";
  meta.sent_time = std::time(nullptr);

  if (!meta.send_on_socket(sock_fd)) {
    std::cerr << "[Client] Failed to send GET_LEADERBOARD request."
              << std::endl;
    return;
  }

#ifdef DEBUG_ON
  std::cout << "[Client::getLeaderboardDataFromServer] Sent request for "
               "leaderboard data."
            << std::endl;
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
  std::cout << "[Client::connectToServer] File receiver thread detached."
            << std::endl;
#endif

  return true;
}

#endif
