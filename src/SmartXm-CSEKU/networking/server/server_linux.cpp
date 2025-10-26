#ifdef __linux__

#include "Message.h"
#include "Server.h"
#include "networking/FileMeta.h"
#include <Users.h>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool Server::running = false;
Server *Server::serverInstance = nullptr;

Server::Server(int port_, const std::string &secret)
    : port(port_), secretKey(secret), status("NOT RUNNING"),
      server_fd(-1) { // constructor
  std::cout << "Local Server Constructor invoked" << std::endl;
  localIP = fetchLocalIP();
  status = "CREATED";
  Server::start();
}

Server::~Server() { // destructor
  std::cout << "Server Destructor invoked" << std::endl;
  stop();
  running = false;
  serverInstance = nullptr;
  status = "DESTROYED";
}

Server *Server::createServer() { // Singleton approach // Create Instance
  if (!running) {
    std::cout << "No server instance found, NEW server starting..."
              << std::endl;
    serverInstance = new Server();
    running = true;
  }
  std::cout << "Server instance exists" << std::endl;
  return Server::serverInstance;
}

int Server::start() { // start server; Constructor will call it only
  if (running)
    return 0;
  std::cout << "Starting server..." << std::endl;
  int opt = 1;
  struct sockaddr_in address;
  // socklen_t addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creation failed");
    status = "FAILED";
    return 1;
  }
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) < 0) {
    perror("setsockopt failed");
    close(server_fd);
    status = "FAILED";
    return 1;
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    status = "FAILED";
    return 1;
  }
  if (listen(server_fd, 3) < 0) {
    perror("listen failed");
    close(server_fd);
    status = "FAILED";
    return 1;
  }

  status = "RUNNING";
  running = true;

  std::cout << "====== SmartXm-CSEKU LOCAL SERVER ======" << std::endl;
  std::cout << "Server will listen on port " << port << std::endl;
  std::cout << "Server local IP (give this to client): " << localIP
            << std::endl;
  // std::cout << "Waiting for client connections..." << std::endl;

  // Start threads
  acceptThread = std::thread(&Server::acceptLoop, this);
  // printerThread = std::thread(&Server::printClientsLoop, this, 5);

  return 0;
}

void Server::stop() {
  if (!running)
    return;
  running = false;
  status = "STOPPED";
  std::cout << "----- STOPPING SERVER -----" << std::endl;

  // Close the listening socket to unblock accept()
  close(server_fd);

  // Close all client sockets
  {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto &ci : clients) {
      close(ci.socfd);
    }
    clients.clear();
  }

  std::cout << "----- SERVER SHOULD BE STOPPED -----" << std::endl;
  return;
}

std::vector<ClientInfo> Server::getClients() {
  std::lock_guard<std::mutex> lock(clientsMutex);
  // Return a copy (snapshot) so callers can iterate without holding the lock.
  return clients;
}

std::string Server::getStatus() { return status; }

std::string Server::getLocalIP() { return localIP; }

void Server::acceptLoop() { // accept new connections
  while (running) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int client_socket =
        accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (!running)
      break; // this !!!
    if (client_socket < 0) {
      if (serverInstance == nullptr)
        perror("failed to accept connection ???");
      continue;
    }

    // AUTHENTICATION:
    char auth_buffer[256] = {0};
    ssize_t received_length =
        recv(client_socket, auth_buffer, sizeof(auth_buffer) - 1, 0);
    if (received_length <= 0) {
      perror("failed to receive secret key");
      close(client_socket);
      continue;
    }
    auth_buffer[received_length] = '\0';
    if (secretKey != std::string(auth_buffer)) {
      std::cout << secretKey << " " << std::string(auth_buffer) << "\n";
      std::cout << "Authentication failed for client from "
                << inet_ntoa(client_addr.sin_addr) << std::endl;
      close(client_socket);
      continue;
    }

    // RECEIVE_CLIENT_INFO:
    char client_name[50] = {0};
    ssize_t name_len =
        recv(client_socket, client_name, sizeof(client_name) - 1, 0);
    if (name_len <= 0) {
      perror("failed to receive client name");
      close(client_socket);
      continue;
    }
    client_name[name_len] = '\0';

    // Add client to list
    ClientInfo ci;
    ci.socfd = client_socket;
    // Make a safe copy of the client's IP string
    ci.ip = std::string(inet_ntoa(client_addr.sin_addr));
    ci.port = ntohs(client_addr.sin_port);
    ci.clientName = std::string(client_name);
    {
      std::lock_guard<std::mutex> lock(clientsMutex);
      clients.push_back(ci);
      // Log the current client count for visibility
      std::cout << "[Server] Client list size: " << clients.size() << std::endl;
    }

    std::cout << "Client '" << client_name << "' connected from " << ci.ip
              << ":" << ci.port << std::endl;

    std::thread client_thread(&Server::handleClient, this, client_socket, ci);
    client_thread.detach();
  }
}

void Server::handleClient(int client_socket, ClientInfo ci) {
  try {
    while (running) {
      // 1. THIS IS THE LOOP: Wait for and receive one FileMeta object.
      // This call will block until a full FileMeta is received.
      FileMeta meta = FileMeta::recv_from_socket(client_socket);

#ifdef DEBUG_ON
      std::cout << "[Server::handleClient] Received packet from socket "
                << client_socket << " with title: " << meta.title << std::endl;
#endif

      // 2. Use the 'title' as a command to decide what to do.
      if (meta.title == "SUBMISSION") {
        // This is a student's submission
        std::cout << "[Server] Received submission: " << meta.filename
                  << " from client " << client_socket << std::endl;

        // --- TODO: Implement submission saving logic ---
        // You need to decide where to save this.
        // For example, create a "submissions" folder.
        // You should use the client's name (from ClientInfo) to name the file.
        // e.g., "submissions/student_email_filename.ext"
        std::string save_path =
            "./submissions/" + meta.filename; // You need a better name
        std::ofstream ofs(save_path, std::ios::binary);
        if (ofs) {
          ofs.write(meta.file_data.data(), meta.file_data.size());
          ofs.close();
          std::cout << "[Server] Saved submission to " << save_path
                    << std::endl;
        } else {
          std::cerr << "[Server] Failed to save submission: " << save_path
                    << std::endl;
        }
      }

      else if (meta.title == "LOGIN_REQ") { // student login
        // This is a login request
        std::cout << "[Server] Received LOGIN request." << std::endl;

        // meta.message probably contains "email:password"
        // --- TODO: Implement login logic ---
        QString L = checkLogin(meta.message);
        FileMeta LL;
        if (L != nullptr) {
          LL.title = "LS";              // Login Succcess
          LL.message = L.toStdString(); // name and student id

          size_t pos = LL.message.find(':');
          std::string n = "";
          std::string i = "";
          if (pos != std::string::npos) {
            n = LL.message.substr(0, pos);
            i = LL.message.substr(pos + 1);
          }
          {
            std::lock_guard<std::mutex> lock(clientsMutex);
            for (ClientInfo &client : clients) {
              if (client.socfd == client_socket) {
                client.clientName = n;
                client.studentID = i;
              }
            }
          }

        } else {
          LL.title = "LF"; // Login Failure
        }
        LL.send_on_socket(client_socket);
      }

      else if (meta.title == "HELLO") {
        // This is a connection test request
        std::cout << "[Server] Received HELLO FILE for testing." << std::endl;

        std::string save_path = "./cache/" + ci.clientName + "_" + ci.ip + "_" +
                                meta.filename; // You need a better name
        std::ofstream ofs(save_path, std::ios::binary);
        if (ofs) {
          ofs.write(meta.file_data.data(), meta.file_data.size());
          ofs.close();
          std::cout << "[Server] Saved hello file to " << save_path
                    << std::endl;
        } else {
          std::cerr << "[Server] Failed to save hello file: " << save_path
                    << std::endl;
        }
      }

      else if (meta.title == "GET_LEADERBOARD") {
        std::cout << "[Server] Received GET_LEADERBOARD request." << std::endl;
        // --- TODO: Implement leaderboard logic ---
        // std::string leaderboard_data = getLeaderboard();
        // send(client_socket, leaderboard_data.c_str(), ...);
      }

      else if (meta.title == "UPDATE_ACCOUNT") { // UNNECESSARY
        std::cout << "[Server] Received UPDATE_ACCOUNT request." << std::endl;
        // --- TODO: Implement account update logic ---
      }

      else {
        std::cerr << "[Server] Received unknown command: " << meta.title
                  << std::endl;
      }
    }
  } catch (const std::exception &e) {
    std::cout << "[Server] Client (socket_id=" << client_socket
              << ") disconnected: " << e.what() << std::endl;
  }

  // --- Client disconnect and cleanup logic (this was already here) ---
  close(client_socket);
  {
    std::lock_guard<std::mutex> lock(clientsMutex);
    auto before = clients.size();
    clients.erase(std::remove_if(clients.begin(), clients.end(),
                                 [client_socket](const ClientInfo &ci) {
                                   return ci.socfd == client_socket;
                                 }),
                  clients.end());
    auto after = clients.size();
    std::cout << "[Server] Removed client socket=" << client_socket
              << ", clients before=" << before << ", after=" << after
              << std::endl;
  }
  std::cout << "Client Handler Thread Terminated!! socket id = "
            << client_socket << std::endl;
}

void Server::printClientsLoop(int intervalSeconds) { // LESS IMPORTANT
  while (running) {
    // while (serverInstance != nullptr) {
    {
      std::lock_guard<std::mutex> lock(clientsMutex);
      std::cout << "-----------------------\nConnected Clients:\n";
      for (const auto &ci : clients) {
        std::cout << "Client: " << ci.ip << ":" << ci.port
                  << ", Name: " << ci.clientName << ", fd: " << ci.socfd
                  << std::endl;
      }
      std::cout << "-----------------------" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
  }
}

std::string Server::fetchLocalIP() {
  struct ifaddrs *ifap, *ifa;
  struct sockaddr_in *sa;
  char *addr;
  std::string found = "error";

  if (getifaddrs(&ifap) == -1) {
    perror("getifaddrs error");
    return found;
  }

  for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
      sa = (struct sockaddr_in *)ifa->ifa_addr;
      addr = inet_ntoa(sa->sin_addr);
      if (strcmp(ifa->ifa_name, "lo") != 0 && strcmp(addr, "127.0.0.1") != 0) {
        found = std::string(addr);
        break;
      }
    }
  }
  freeifaddrs(ifap);
  return found;
}

// send to specific client
// kivabe button e implement korbo bujhtesi na
bool Server::sendFileToClient(int client_sock, const std::string path,
                              const std::string title, const std::string msg) {
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open())
    return false;
  file.seekg(0, std::ios::end);
  size_t sz = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<char> filedata(sz);
  file.read(filedata.data(), sz);

  std::string fname = std::filesystem::path(path).filename();
  std::string ext = std::filesystem::path(path).extension().string();
  if (ext.size() && ext[0] == '.')
    ext = ext.substr(1);

  FileMeta meta(title, fname, ext, std::time(nullptr), std::move(filedata),
                msg);
  return meta.send_on_socket(client_sock);
}

// Broadcast to all // might be easier
bool Server::sendFileToAllClients(const FileMeta &meta) {
  // Snapshot sockets
  std::vector<int> sockets;
  {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (const auto &c : clients)
      sockets.push_back(c.socfd);
  }
  bool all_ok = true;
  std::vector<std::thread> workers;
  for (int sock : sockets) {
    workers.emplace_back([&, sock]() {
      // meta.send_on_socket(sock);
      if (!meta.send_on_socket(sock)) {
        std::cerr << "Failed to send FileMeta to client socket: " << sock
                  << "\n";
        all_ok = false;
      }
    });
  }
  for (auto &t : workers)
    t.detach();
  return all_ok;
}

// Receive from a client (example, e.g. for submissions)
FileMeta Server::receiveFileFromClient(int client_sock) {
  return FileMeta::recv_from_socket(client_sock);
}

#include <db_xampp.h>
QString Server::checkLogin(std::string str) {
  size_t pos = str.find(':');
  std::string e = "";
  std::string p = "";
  if (pos != std::string::npos) {
    e = str.substr(0, pos);
    p = str.substr(pos + 1);
  }
  localDB *ddbb = localDB::DB();
  QSqlQuery q = ddbb->execQuery(
      "SELECT * FROM Users WHERE Users.email=\"" + QString::fromStdString(e) +
      "\" AND Users.password=\"" + QString::fromStdString(p) + "\";");
  if (q.isActive() && q.size() == 1) {
    while (q.next()) {
      std::string name = q.value("name").toString().toStdString();
      std::string email = q.value("email").toString().toStdString();
      std::string pass = q.value("password").toString().toStdString();
      std::string id = q.value("id").toString().toStdString();
      if (email == e && pass == p) {
        return QString(q.value("name").toString() + ":" +
                       q.value("id").toString());
      } else {
        return nullptr;
      }
    }
  } else if (q.isActive() && q.size() == 0) {
    std::cout << "[Server:DB] No data found." << std::endl;
  } else if (q.isActive() && q.size() > 1) {
    std::cout << "Database should not contain duplicates." << std::endl;
  } else {
    std::cout << "No data found or query was inactive." << std::endl;
  }
  std::cout << "[Server:checkLogin] ERROR" << std::endl;
  return nullptr;
}

#endif
