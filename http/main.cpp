#include <iostream>
#include <memory.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>

#include "net/http_interface.h"
#include "net/http_server.h"


void set_response(std::string* buf, char*&& query)
{
    std::string default_path = "./templates/main";
    const char* path = default_path.append(query).c_str();

    if (strcmp(query, "/") == 0)
    {
        const char* path = default_path.append("/index.html").c_str();
    }

    std::ifstream i_stream{path};
    if (i_stream.is_open())
    {
        std::string line;
        while (std::getline(i_stream, line))
        {
            *buf += line;
        }
    }
    else
    {
        i_stream.close();
        std::ifstream i_stream{"./templates/main/404.html"};
        std::string line;
        while (std::getline(i_stream, line))
        {
            *buf += line;
        }
    }
    i_stream.close();
}

void request_handler(int& sockfd, char*&& first_line)
{
    std::string buf = "HTTP/1.1 200 ok\r\n\n";

    char* method = strtok(first_line, " ");
    char* query = strtok(NULL, " ");
    char* version = strtok(NULL, "\r\n");

    if (strcmp(method, "GET") == 0)
    {
        set_response(&buf, std::move(query));
    }
    else if (strcmp(method, "POST") == 0)
    {
        //post_handler();
    }
    send(sockfd, buf.c_str(), buf.size(), 0);
}

void connection(int&& sockfd, std::mutex& mtx)
{
    printf("connected.\n");

    char buf[64];
    std::string msg;
    while (true)
    {
        memset(buf, 0x00, 64);
        int retval = recv(sockfd, buf, 64, 0);
        if(retval < 64)
        {
            msg.append(buf);
            break;
        }
        else if (retval < 1)
        {
            close(sockfd);
            return;
        }
        msg.append(buf);
    }
    char tmp[msg.length()];
    memset(tmp, 0x00, sizeof(tmp));
    strcpy(tmp, msg.c_str());

    char* first_line = strtok(tmp, "\r\n");
    request_handler(sockfd, std::move(first_line));

    close(sockfd);
}

int main()
{
    const char* localhost = "127.0.0.1";
    const int   port      = 7777;

    std::queue<std::thread> conn_threads;
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
        conn_threads.push(
            std::thread(connection, std::move(client_socket), std::ref(mtx))
        );
        printf("thread 수: %ld", conn_threads.size());
    }

    std::cout << "Server end." << std::endl;

    return 0;
}
