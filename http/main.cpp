#include <iostream>
#include <string>
#include <fstream>
#include <memory.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "net/http_interface.h"
#include "net/http_server.h"

#define __WORKER_SIZE          (10)
#define __CONN_REQUESTS_LIMITS (10)


size_t                  conn_requests_size = 0;
std::queue<int>         conn_requests;
std::mutex              mtx;
std::condition_variable cond_var;


void set_response(std::string* buf, const char* query)
{
    std::string default_path = "./templates/main";
    const char* path = default_path.append(query).c_str();

    if (strcmp(query, "/") == 0)
    {
        path = default_path.append("/index.html").c_str();
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

void request_handler(int& sockfd, std::string&& msg)
{
    char tmp[msg.length()];
    memset(tmp, 0x00, sizeof(tmp));
    strcpy(tmp, msg.c_str());

    char* first_line = strtok(tmp, "\r\n");
    const char* method = strtok(first_line, " ");
    const char* query = strtok(NULL, " ");
    const char* version = strtok(NULL, "\r\n");

    std::string buf = "HTTP/1.1 200 ok\r\n\n";

    if (strcmp(method, "GET") == 0)
    {
        set_response(&buf, query);
    }
    else if (strcmp(method, "POST") == 0)
    {
        size_t idx = msg.rfind('\n');
        std::string body = msg.substr(idx + 1, msg.size() - 1);
        std::string name = body.substr(body.find('=') + 1, body.find('&') - body.find('=') - 1);
        std::string snumber = body.substr(body.rfind('=') + 1);
        buf += "<!DOCTYPE html><html><header><title>sample</title></header><body><h1>Hello, ";
        buf += name;
        buf += "!</h1><br>student number: ";
        buf += snumber;
        buf += "</body></html>";
    }
    send(sockfd, buf.c_str(), buf.size(), 0);
}

void worker()
{
first:
    while (true)
    {
        std::unique_lock<std::mutex> lck(mtx);
        cond_var.wait(lck,
                      [&] { return !conn_requests.empty(); });

        int sockfd = conn_requests.front();
        conn_requests.pop();
        conn_requests_size--;
        lck.unlock();

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
                goto first;
            }
            msg.append(buf);
        }
        request_handler(sockfd, std::move(msg));

        close(sockfd);
    }
}

int main()
{
    const char* localhost = "127.0.0.1";
    const int   port      = 7777;

    auto http_server = jhleeeme::net::HttpServer();
    http_server.create_socket();
    http_server.set_sockaddr(localhost, port);
    http_server.bind();
    http_server.listen();

    std::vector<std::thread> workers;
    for (int i = 0; i < __WORKER_SIZE; i++)
    {
        workers.push_back(std::thread(worker));
    }

    while (true)
    {
        int client_socket = http_server.accept();
        sockaddr_in client_sockaddr = http_server.get_client_sockaddr();
        printf("Connection request: %s:%d\n",
               inet_ntoa(client_sockaddr.sin_addr), ntohs(client_sockaddr.sin_port));

        std::lock_guard<std::mutex> lock(mtx);
        if (conn_requests_size < __CONN_REQUESTS_LIMITS)
        {
            conn_requests.push(std::move(client_socket));
            conn_requests_size++;
            cond_var.notify_one();
        }
        else
        {
            const char* resp = "Connection 대기열 꽉참!";
            send(client_socket, resp, strlen(resp), 0);
            close(client_socket);
        }
    }

    std::cout << "Server end." << std::endl;

    return 0;
}
