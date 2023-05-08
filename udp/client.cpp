#include <iostream>
#include <memory.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define __SERVER_IP ("127.0.0.1")
#define __PORT (7777)

int main()
{
    std::cout << "Client start!" << std::endl;

    int client_socket;
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0)
    {
        std::cerr << "socket() failed." << std::endl;
        return -1;
    }

    sockaddr_in s_sockaddr_info;
    memset(&s_sockaddr_info, 0x00, sizeof(s_sockaddr_info));
    s_sockaddr_info.sin_family = AF_INET;
    s_sockaddr_info.sin_addr.s_addr = inet_addr(__SERVER_IP);
    s_sockaddr_info.sin_port = htons(__PORT);
    const char* msg = "Hello, UDP!";

    if (sendto(client_socket, msg, strlen(msg), 0,
               (sockaddr*)&s_sockaddr_info, sizeof(s_sockaddr_info)) < 0)
    {
        std::cerr << "sendto() failed." << std::endl;
        return -1;
    }

    std::cout << "Client end.." << std::endl;

    close(client_socket);

    return 0;
}

