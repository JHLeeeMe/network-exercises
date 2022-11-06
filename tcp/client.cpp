#include <iostream>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//#define __SERVER_IP ("142.250.207.14")  // google.com
#define __SERVER_IP ("127.0.0.1")
#define __PORT (7777)
//#define __PORT (80)

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
    //inet_aton(__SERVER_IP, &s_sockaddr_info.sin_addr);
    s_sockaddr_info.sin_addr.s_addr = inet_addr(__SERVER_IP);
    s_sockaddr_info.sin_port = htons(__PORT);

    if (connect(client_socket, (sockaddr*)&s_sockaddr_info, sizeof(s_sockaddr_info)) < 0)
    {
        std::cerr << "connect() failed" << std::endl;
        return -1;
    }

    //const char* msg = "hello~!" ;
    //if (send(client_socket, msg, strlen(msg), 0) < 0)
    char msg[32] = "hello~!";
    //char msg[32] = "GET / HTTP/1.1\r\n\r\n";
    if (send(client_socket, msg, sizeof(msg), 0) < 0)
    {
        std::cerr << "send() failed" << std::endl;
        return -1;
    }

    std::cout << "Sending completed!" << std::endl;

    // for google.com
    //char resp_buf[4096];
    //if (recv(client_socket, resp_buf, sizeof(resp_buf), 0) < 0)
    //{
    //    std::cerr << "recv() failed" << std::endl;
    //    return -1;
    //}

    //std::cout << resp_buf << std::endl;

    close(client_socket);

    std::cout << "Client end!" << std::endl;

    return 0;
}
