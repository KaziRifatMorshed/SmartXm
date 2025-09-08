#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ios>
#include <iostream>
#include <limits>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "Msg.h"
using namespace std;

#define PORT 8080
const char* SECRET_KEY = "MySuperSecret123x";

void error(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, const char* argv[]) {
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
    cout << "ip_addr = " << ip_addr << endl;
    if (inet_pton(AF_INET, ip_addr, &server_addr.sin_addr) <= 0) {
        error("invalid address or address not found");
    }

    // connect to the server
    if (connect(client_sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        error("connection failed");
    }
    else {
        cout << "Connected to server at: " << ip_addr << " : " << PORT << endl;

        cout << "Enter your name: ";
        cin.getline(client_name, sizeof(client_name));
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

    AUTHENTICATION:
        // After connecting to server
        if (send(client_sock_fd, SECRET_KEY, strlen(SECRET_KEY), 0) < 0) {
            error("Authentication failed");
        }
        // connect houar por secret key poathiye check korbe je amar dol er lok ki na

        // Send client name to server (ensure null-terminated)
        if (send(client_sock_fd, client_name, strlen(client_name), 0) < 0) {
            error("Client information sent failed");
        }
    }

    cout << "\nNow, you should be able to chat with the server..." << endl;

    while (true) {
        cout << "Write Message (and hit Enter): ";

        char temp[BUFFER_SIZE] = {0};
        cin.getline(temp, BUFFER_SIZE);

        if (cin.fail()) {
            cout << "INPUT IS TOO LONG !!! Only first " << (BUFFER_SIZE - 1) << " characters will be used." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (strcmp(temp, "\\c") == 0) {
            cout << "Closing chatting..." << endl;
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
            cout << "ACK: Msg sent to server : " << temp << endl;
        }
    }

    close(client_sock_fd);
    cout << "Client has been closed" << endl;
}
