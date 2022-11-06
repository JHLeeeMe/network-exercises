#include <iostream>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define __PORT (7777)
#define __BUF_SIZE (1024)
#define __BACKLOG_QUEUE_SIZE (1)

int main()
{
    std::cout << "Server start!" << std::endl;

    /*
     *  -------------------------------------------
     *            Create server socket
     *  -------------------------------------------
     */
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << "Can't create server socket" << std::endl;
        return -1;
    }

    /*
     *  -------------------------------------------
     *                 Bind ip/port
     *  -------------------------------------------
     */
    sockaddr_in s_sockaddr_info;
    s_sockaddr_info.sin_family = AF_INET;
    s_sockaddr_info.sin_addr.s_addr = INADDR_ANY;
    s_sockaddr_info.sin_port = htons(__PORT);

    if (bind(server_socket, (sockaddr*)&s_sockaddr_info, sizeof(s_sockaddr_info)) < 0)
    {
        std::cerr << "Can't bind ip/port to server socket" << std::endl;
        return -1;
    }

    /*
     *  -------------------------------------------
     *               Listen connection
     *  -------------------------------------------
     */
    if (listen(server_socket, __BACKLOG_QUEUE_SIZE) < 0)
    {
        std::cerr << "listen() failed." << std::endl;
        return -1;
    }

    std::cout << "Waiting..." << std::endl;

    /*
     *  -------------------------------------------
     *               Accept connection
     *  -------------------------------------------
     */
    sockaddr_in c_sockaddr_info;
    socklen_t c_sockaddr_info_len = sizeof(c_sockaddr_info);
    int client_socket = accept(server_socket, (sockaddr*)&s_sockaddr_info, &c_sockaddr_info_len);
    if (client_socket < 0)
    {
        std::cerr << "accpet() failed." << std::endl;
        return -1;
    }
    else if (client_socket == 0)
    {
        std::cout << "client disconnect." << std::endl;
        return -1;
    }

    std::cout << "Client is connected!" << std::endl;

    /*
     *  -------------------------------------------
     *                  Recv data
     *  -------------------------------------------
     */
    char buf[__BUF_SIZE];
    memset(buf, 0x00, __BUF_SIZE);

    recv(client_socket, buf, __BUF_SIZE, 0);
    std::cout << buf << std::endl;

    /*
     *  -------------------------------------------
     *               Close socket
     *  -------------------------------------------
     */
    close(client_socket);
    close(server_socket);

    std::cout << "Server end." << std::endl;

    return 0;
}
