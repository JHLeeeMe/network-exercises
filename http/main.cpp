#include <iostream>
#include <memory.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include <string>

#include "net/http_interface.h"
#include "net/http_server.h"


void connection(int&& sockfd, std::mutex& mtx)
{
    printf("connected.\n");

    char buf[64];
    std::string msg;
    while (true)
    {
        memset(buf, 0x00, 64);
        if(recv(sockfd, buf, 64, 0) < 64)
        {
            msg.append(buf);
            break;
        }
        msg.append(buf);
    }
    char tmp[msg.length()];
    memset(tmp, 0x00, sizeof(tmp));
    strcpy(tmp, msg.c_str());

    char* first_line = strtok(tmp, "\r\n");
    char* method = strtok(first_line, " ");
    char* query = strtok(NULL, " ");
    char* version = strtok(NULL, "\r\n");
    printf("%s\n", method);
    printf("%s\n", query);
    printf("%s\n", version);

    close(sockfd);
}

int main()
{
    const char* localhost = "127.0.0.1";
    const int   port      = 7777;

    std::vector<std::thread> conn_threads;
    conn_threads.reserve(10);
    std::mutex mtx;

    auto http_server = jhleeeme::net::HttpServer();
    http_server.create_socket();
    http_server.set_sockaddr(localhost, port);
    http_server.bind();
    http_server.listen();

    while (true)
    {
        int client_socket = http_server.accept();
        sockaddr_in client_sockaddr = http_server.get_client_sockaddr();
        printf("Connection request: %s:%d\n",
               inet_ntoa(client_sockaddr.sin_addr), ntohs(client_sockaddr.sin_port));
        conn_threads.push_back(
            std::thread(connection, std::move(client_socket), std::ref(mtx))
        );
    }

    std::cout << "Server end." << std::endl;

    return 0;
}
