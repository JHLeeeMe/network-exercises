#include <sys/socket.h>
#include <memory.h>
#include <unistd.h>

#include "http_base.h"


namespace jhleeeme
{
namespace net
{

HttpBase::HttpBase()
    : _server_socket(0)
{
    ::memset(&_server_sockaddr, 0x00, sizeof(_server_sockaddr));
}

void HttpBase::create_socket()
{
    _server_socket = socket(AF_INET, SOCK_STREAM, 0);
}

void HttpBase::set_server_sockaddr(const char* ip, int port)
{
    _server_sockaddr.sin_family = AF_INET;
    _server_sockaddr.sin_addr.s_addr = inet_addr(ip);
    _server_sockaddr.sin_port = htons(port);
}

int HttpBase::bind()
{
    return ::bind(_server_socket, (struct sockaddr*)&_server_sockaddr, sizeof(_server_sockaddr));
}

void HttpBase::close()
{
    ::close(_server_socket);
}

}
}
