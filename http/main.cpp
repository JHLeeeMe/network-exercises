#include <iostream>
#include <memory.h>
#include <unistd.h>

#include "net/http_interface.h"
#include "net/http_server.h"
#include "net/http_client.h"

int main()
{
    const char* localhost = "127.0.0.1";
    const int   port      = 7777;

    //jhleeeme::net::IHttp* http_server = new jhleeeme::net::HttpServer();
    //http_server->create_socket();
    //std::cout << http_server->get_sockfd() << std::endl;
    auto http_server = jhleeeme::net::HttpServer();
    http_server.create_socket();
    http_server.set_sockaddr(localhost, port);
    http_server.bind();
    http_server.listen();

    std::cout << "Waiting..." << std::endl;

    int client_socket = http_server.accept();
    std::cout << "connected." << std::endl;
    http_server.close();

    char buf[4096];
    memset(buf, 0x00, 4096);

    recv(client_socket, buf, 4096, 0);
    std::cout << buf << std::endl;

    close(client_socket);

    std::cout << "Server end." << std::endl;

    return 0;
}
