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
    virtual void set_server_sockaddr(const char* ip, int port);
    virtual int bind();
    virtual int listen()=0;
    virtual int accept();
    virtual int send();
    virtual int recv();
    void close();
private:
    int _server_socket;
    sockaddr_in _server_sockaddr;
    int _client_socket;
    sockaddr_in _client_sockaddr;
};

}
}
