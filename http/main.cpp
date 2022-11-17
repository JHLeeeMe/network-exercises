#include <iostream>
#include <memory.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>

#include "net/http_interface.h"
#include "net/http_server.h"
#include "net/http_client.h"


void connection(int&& sockfd, std::mutex& mtx)
{
    std::cout << "connected." << std::endl;

    char buf[64];
    while (true)
    {
        memset(buf, 0x00, 64);

        recv(sockfd, buf, 64, 0);
        std::cout << buf;
    }
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
        conn_threads.push_back(
            std::thread(connection, std::move(client_socket), std::ref(mtx))
        );
    }

    std::cout << "Server end." << std::endl;

    return 0;
}
