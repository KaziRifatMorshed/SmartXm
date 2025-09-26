// client code
// put server ip address in argv

#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include "Msg.h"

#define PORT 8080
#define BUFFER_SIZE 1024
const char* SECRET_KEY = "MySuperSecret123x";

void error(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Structure to hold client information
struct ClientInfo
{
    int socket;
    std::string name;
};

// Function to receive files from the server
void receive_file(int client_sock_fd) {
    while (true) {
        size_t file_size;
        ssize_t bytes_received = recv(client_sock_fd, &file_size, sizeof(file_size), MSG_WAITALL);
        if (bytes_received <= 0) {
            std::cerr << "Server disconnected or error receiving file size." << std::endl;
            break;
        }

        std::cout << "Receiving file of size: " << file_size << " bytes." << std::endl;

        char* file_buffer = new char[file_size];
        bytes_received = recv(client_sock_fd, file_buffer, file_size, MSG_WAITALL);
        if (bytes_received != file_size) {
            std::cerr << "Error receiving file data. Expected: " << file_size << ", received: " << bytes_received
                      << std::endl;
            delete[] file_buffer;
            break;
        }

        std::string filename = "received_file.tar";
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


int _main(int argc, const char* argv[]) {
    int client_sock_fd = 0;
    struct sockaddr_in server_addr;
    char client_name[50] = {0};

    // create socket
    if ((client_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Socket creation error");
    }

    // prepare server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    const char* ip_addr = (argc == 2 && strlen(argv[1]) > 0) ? argv[1] : "127.0.0.1";
    std::cout << "ip_addr = " << ip_addr << std::endl;
    if (inet_pton(AF_INET, ip_addr, &server_addr.sin_addr) <= 0) {
        error("invalid address or address not found");
    }

    // connect to the server
    if (connect(client_sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        error("connection failed");
    }
    else {
        std::cout << "Connected to server at: " << ip_addr << " : " << PORT << std::endl;

        std::cout << "Enter your name: ";
        std::cin.getline(client_name, sizeof(client_name));
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // AUTHENTICATION:
        //  After connecting to server
        if (send(client_sock_fd, SECRET_KEY, strlen(SECRET_KEY), 0) < 0) {
            error("Authentication failed");
        }
        // connect houar por secret key poathiye check korbe je amar dol er lok ki na

        // Send client name to server (ensure null-terminated)
        if (send(client_sock_fd, client_name, strlen(client_name), 0) < 0) {
            error("Client information sent failed");
        }
    }

    std::cout << "\nNow, you should be able to chat with the server..." << std::endl;

    // Start the file receiving thread
    std::thread file_receiver(receive_file, client_sock_fd);
    file_receiver.detach();

    while (true) {
        std::cout << "Write Message (and hit Enter): ";

        char temp[BUFFER_SIZE] = {0};
        std::cin.getline(temp, BUFFER_SIZE);


        if (std::cin.fail()) {
            std::cout << "INPUT IS TOO LONG !!! Only first " << (BUFFER_SIZE - 1) << " characters will be used."
                      << std::endl;
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

        if (send(client_sock_fd, &msg, sizeof(msg), 0) < 0) {
            error("msg sent failed !!!!!");
        }
        else {
            std::cout << "ACK: Msg sent to server : " << temp << std::endl;
        }
    }

    close(client_sock_fd);
    std::cout << "Client has been closed" << std::endl;

    return 0;
}
