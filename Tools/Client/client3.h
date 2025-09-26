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

void error(const char* msg);

// Structure to hold client information
struct ClientInfo
{
    int socket;
    std::string name;
};

// Function to receive files from the server
void receive_file(int client_sock_fd);


int initClient(std::string ip);
