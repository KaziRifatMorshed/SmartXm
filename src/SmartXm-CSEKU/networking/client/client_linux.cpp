#ifdef __linux__

#include "Client.h"

Client::Client() : sock_fd(-1), connected(false) {}

Client::~Client() {
    disconnect();
}

bool Client::connectToServer(const std::string& ip_addr, int port) {
    std::lock_guard<std::mutex> lock(connMutex);
    if (connected) {
        std::cerr << "Already connected!" << std::endl;
        return false;
    }

    struct sockaddr_in server_addr;
    char name_buffer[50] = {0};

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return false;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_addr.c_str(), &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return false;
    }

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return false;
    }

    std::cout << "Connected to server at: " << ip_addr << " : " << port << std::endl;

    std::cout << "Enter your name: ";
    std::cin.getline(name_buffer, sizeof(name_buffer));
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    clientName = name_buffer;

           // AUTHENTICATION
    if (send(sock_fd, CLIENT_SECRET_KEY, strlen(CLIENT_SECRET_KEY), 0) < 0) {
        perror("Authentication failed");
        close(sock_fd);
        sock_fd = -1;
        return false;
    }
    // Send client name
    if (send(sock_fd, clientName.c_str(), clientName.size(), 0) < 0) {
        perror("Sending client name failed");
        close(sock_fd);
        sock_fd = -1;
        return false;
    }

    connected = true;
    // Start file receiving thread
    fileReceiverThread = std::thread(&Client::receiveFileLoop, this);
    fileReceiverThread.detach();
    return true;
}

void Client::chatLoop() {
    if (!connected) {
        std::cerr << "Not connected to server!" << std::endl;
        return;
    }

    char temp[CLIENT_BUFFER_SIZE] = {0};
    while (true) {
        std::cout << "Write Message (and hit Enter): ";
        std::cin.getline(temp, CLIENT_BUFFER_SIZE);

        if (std::cin.fail()) {
            std::cout << "INPUT IS TOO LONG!!! Only first " << (CLIENT_BUFFER_SIZE - 1)
            << " characters will be used." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (strcmp(temp, "\\c") == 0) {
            std::cout << "Closing chatting..." << std::endl;
            break;
        }

        Msg msg;
        strncpy(msg.sender_name, clientName.c_str(), sizeof(msg.sender_name) - 1);
        msg.sender_name[sizeof(msg.sender_name) - 1] = '\0';
        strncpy(msg.text, temp, sizeof(msg.text) - 1);
        msg.text[sizeof(msg.text) - 1] = '\0';

        if (send(sock_fd, &msg, sizeof(msg), 0) < 0) {
            perror("Msg send failed");
        } else {
            std::cout << "ACK: Msg sent to server : " << temp << std::endl;
        }
    }
    disconnect();
}

void Client::receiveFileLoop() {
    while (connected) {
        size_t file_size;
        ssize_t bytes_received = recv(sock_fd, &file_size, sizeof(file_size), MSG_WAITALL);
        if (bytes_received <= 0) {
            std::cerr << "Server disconnected or error receiving file size." << std::endl;
            break;
        }

        std::cout << "Receiving file of size: " << file_size << " bytes." << std::endl;

        char* file_buffer = new char[file_size];
        bytes_received = recv(sock_fd, file_buffer, file_size, MSG_WAITALL);
        if (bytes_received != (ssize_t)file_size) {
            std::cerr << "Error receiving file data. Expected: " << file_size
                      << ", received: " << bytes_received << std::endl;
            delete[] file_buffer;
            break;
        }

        std::string filename = "received_file";
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

void Client::disconnect() {
    std::lock_guard<std::mutex> lock(connMutex);
    if (connected) {
        close(sock_fd);
        sock_fd = -1;
        connected = false;
        std::cout << "Client has been closed" << std::endl;
    }
}

#endif
