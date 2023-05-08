#include <iostream>
#include <memory.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define __PORT (7777)
#define __BACKLOG_QUEUE_SIZE (1)
#define __BUF_SIZE (1024)

int main()
{
    int server_socket;
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0)
    {
        std::cout << "socket() failed." << std::endl;
        return -1;
    }

    sockaddr_in s_sockaddr_info;
    memset(&s_sockaddr_info, 0x00, sizeof(s_sockaddr_info));
    s_sockaddr_info.sin_family = AF_INET;
    s_sockaddr_info.sin_addr.s_addr = htonl(INADDR_ANY);
    s_sockaddr_info.sin_port = htons(__PORT);

    if (bind(server_socket, (sockaddr*)&s_sockaddr_info, sizeof(s_sockaddr_info)) < 0)
    {
        std::cout << "bind() failed." << std::endl;
        return -1;
    }

    std::cout << "Waiting.." << std::endl;

    char recv_buf[__BUF_SIZE];
    memset(recv_buf, 0x00, __BUF_SIZE);
    sockaddr_in c_sockaddr_info;
    socklen_t c_sockaddr_info_len = sizeof(c_sockaddr_info);
    memset(&c_sockaddr_info, 0x00, c_sockaddr_info_len);

    recvfrom(server_socket, recv_buf, __BUF_SIZE, 0,
             (sockaddr*)&c_sockaddr_info, &c_sockaddr_info_len);

    std::cout << "Recv buf: " << recv_buf << std::endl;

    close(server_socket);

    std::cout << "Server end.." << std::endl;

    return 0;
}

