#pragma once

#include <arpa/inet.h>


namespace jhleeeme
{
namespace net
{

class HttpBase
{
public:
    HttpBase();
    virtual ~HttpBase()=default;
public:
    virtual void create_socket();
    virtual void set_sockaddr(const char* ip, int port);
    virtual int bind();
    virtual int listen()=0;
private:
    int _sockfd;
    sockaddr_in _sockaddr;
};

}
}
