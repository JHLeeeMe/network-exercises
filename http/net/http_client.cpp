#include <memory.h>
#include <unistd.h>

#include "http_client.h"

namespace jhleeeme
{
namespace net
{

HttpClient::HttpClient()
    : HttpBase()
    , _sockfd(-1)
{
    memset(&_sockaddr, 0x00, sizeof(_sockaddr));
}

HttpClient::~HttpClient()
{
    close();
    memset(&_sockaddr, 0x00, sizeof(_sockaddr));
}

void HttpClient::create_socket()
{
    _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
}

int HttpClient::bind()
{
    return 0;
}
int HttpClient::listen()
{
    return 0;
}

int HttpClient::accept()
{
    return 0;
}

void HttpClient::close()
{
    ::close(_sockfd);
}

const int& HttpClient::get_sockfd() const
{
    return _sockfd;
}

void HttpClient::set_sockaddr(const char* ip, const int port)
{
    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = inet_addr(ip);
    _sockaddr.sin_port = htons(port);
}

}
}
