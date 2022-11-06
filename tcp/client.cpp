#include <iostream>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define __PORT (7777)

int main()
{
    std::cout << "Client start!" << std::endl;

    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        std::cerr << "Can't create client socket" << std::endl;
        return -1;
    }

    sockaddr_in s_sockaddr_info;
    memset(&s_sockaddr_info, 0x00, sizeof(s_sockaddr_info));
    s_sockaddr_info.sin_family = AF_INET;
    //inet_aton("127.0.0.1", &s_sockaddr_info.sin_addr);
    s_sockaddr_info.sin_addr.s_addr = inet_addr("127.0.0.1");
    s_sockaddr_info.sin_port = htons(__PORT);

    if (connect(client_socket, (sockaddr*)&s_sockaddr_info, sizeof(s_sockaddr_info)) < 0)
    {
        std::cerr << "connect() failed" << std::endl;
        return -1;
    }

    //const char* msg = "hello~!" ;
    //if (send(client_socket, msg, strlen(msg), 0) < 0)
    char msg[32] = "hello~!";
    if (send(client_socket, msg, sizeof(msg), 0) < 0)
    {
        std::cerr << "send() failed" << std::endl;
        return -1;
    }

    std::cout << "Sending completed!" << std::endl;
    close(client_socket);

    std::cout << "Client end!" << std::endl;

    return 0;
}
