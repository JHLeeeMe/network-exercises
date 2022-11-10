#include <iostream>

#include "net/http_base.h"
#include "net/http_server.h"

int main()
{
    //jhleeeme::net::HttpBase* http_server = new jhleeeme::net::HttpServer();
    //http_server->create_socket();
    //std::cout << http_server->get_sockfd() << std::endl;
    auto http_server = jhleeeme::net::HttpServer();
    http_server.create_socket();
    std::cout << http_server.get_sockfd() << std::endl;

    return 0;
}
