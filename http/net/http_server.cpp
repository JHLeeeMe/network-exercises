#include <memory.h>
#include <utility>
#include <unistd.h>

#include "http_server.h"

namespace jhleeeme
{
namespace net
{

HttpServer::HttpServer()
    : IHttp()
    , _sockfd(-1)
    , _client_sockaddr_len(sizeof(_client_sockaddr))
{
    memset(&_sockaddr, 0x00, sizeof(_sockaddr));
    memset(&_client_sockaddr, 0x00, _client_sockaddr_len);
}

HttpServer::~HttpServer()
{
    close();
    memset(&_sockaddr, 0x00, sizeof(_sockaddr));
    memset(&_client_sockaddr, 0x00, _client_sockaddr_len);
    _client_sockaddr_len = 0;
}

void HttpServer::create_socket()
{
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
}

int HttpServer::bind()
{
    return ::bind(_sockfd, (sockaddr*)&_sockaddr, sizeof(_sockaddr));
}

int HttpServer::listen()
{
    return ::listen(_sockfd, __BACKLOG_QUEUE_SIZE);
}

int HttpServer::accept()
{
    return ::accept(_sockfd, (sockaddr*)&_client_sockaddr, &_client_sockaddr_len);
}

void HttpServer::close()
{
    ::close(_sockfd);
}

const int& HttpServer::get_sockfd() const
{
    return _sockfd;
}

void HttpServer::set_sockaddr(const char* ip, const int port)
{
    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = inet_addr(ip);
    _sockaddr.sin_port = htons(port);
}

}
}
