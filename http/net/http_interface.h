#pragma once

#include <arpa/inet.h>

#define __BACKLOG_QUEUE_SIZE (10)


namespace jhleeeme
{
namespace net
{

class HttpBase
{
public:
    HttpBase()=default;
    virtual ~HttpBase()=default;
public:
    virtual void        create_socket()=0;
    virtual int         bind()=0;
    virtual int         listen()=0;
    virtual int         accept()=0;
    virtual void        close()=0;

    virtual const int&  get_sockfd() const=0;
    virtual void        set_sockaddr(const char* ip, const int port)=0;
};

}
}
