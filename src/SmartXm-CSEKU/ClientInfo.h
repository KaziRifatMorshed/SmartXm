#include <string>
#ifndef CLIENT_INFO_CLASS
#define CLIENT_INFO_CLASS
class ClientInfo
{
public:
    int socfd; // socket file discriptor
    std::string ip;
    std::string clientName;
    std::string studentID;
    int port;
};
#endif
