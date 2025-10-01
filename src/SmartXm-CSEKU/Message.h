#ifndef MESSAGE
#define MESSAGE

#define BUFFER_SIZE 1024

class Message
{
public:
    char sender_name[50];
    char text[BUFFER_SIZE];
};

#endif
