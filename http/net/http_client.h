#pragma once

#include "http_interface.h"

namespace jhleeeme
{
namespace net
{

class HttpClient : public IHttp
{
public:
    HttpClient();
    ~HttpClient() override;
public:
    void       create_socket() override;
    int        bind() override;
    int        listen() override;
    int        accept() override;
    void       close() override;

    const int& get_sockfd() const override;
    void       set_sockaddr(const char* ip, const int port) override;
private:
    int         _sockfd;
    sockaddr_in _sockaddr;
};

}
}
