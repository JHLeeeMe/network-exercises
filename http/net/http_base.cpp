#include <sys/socket.h>
#include <memory.h>

#include "http_base.h"


namespace jhleeeme
{
namespace net
{

HttpBase::HttpBase()
    : _sockfd(0)
{
    ::memset(&_sockaddr, 0x00, sizeof(_sockaddr));
}

void HttpBase::create_socket()
{
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
}

void HttpBase::set_sockaddr(const char* ip, int port)
{
    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = inet_addr(ip);
    _sockaddr.sin_port = htons(port);
}

int HttpBase::bind()
{
    return ::bind(_sockfd, (struct sockaddr*)&_sockaddr, sizeof(_sockaddr));
}

}
}
