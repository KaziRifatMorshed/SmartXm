#ifndef MSG
#define MSG

#define BUFFER_SIZE 1024

class Msg
{
public:
    char sender_name[50];
    char text[BUFFER_SIZE];
};

#endif
