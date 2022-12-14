#pragma once

#include "http_interface.h"

namespace jhleeeme
{
namespace net
{

class HttpServer : public IHttp
{
public:
    HttpServer();
    ~HttpServer() override;
public:
    void               create_socket() override;
    int                bind() override;
    int                listen() override;
    int                accept() override;
    void               close() override;

    const int&         get_sockfd() const override;
    void               set_sockaddr(const char* ip, const int port) override;
    const sockaddr_in& get_client_sockaddr() const;
private:
    int                _sockfd;
    sockaddr_in        _sockaddr;
    sockaddr_in        _client_sockaddr;
    socklen_t          _client_sockaddr_len;
};

}
}
