#include <iostream>

#include "net/http_interface.h"
#include "net/http_server.h"
#include "net/http_client.h"

int main()
{
    //jhleeeme::net::IHttp* http_server = new jhleeeme::net::HttpServer();
    //http_server->create_socket();
    //std::cout << http_server->get_sockfd() << std::endl;
    auto http_server = jhleeeme::net::HttpServer();
    http_server.create_socket();
    std::cout << http_server.get_sockfd() << std::endl;

    return 0;
}
