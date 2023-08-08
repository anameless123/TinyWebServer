#pragma once
#include <sys/socket.h>
#include "utils.hpp"

class InetAddress;
class Socket
{
private:
    int sockfd = 0;
    // IP ver, default = IPv4;
    int IP = AF_INET;
    // Max Connection, default = 128;
    int MaxCon = 128;

public:
    Socket();
    Socket(int sockfd);
    Socket(__socket_type tran, uint16_t inet);
    ~Socket();
    void bind(InetAddress addr);
    void listen();
    void connect(InetAddress addr);
    int accept(InetAddress *addr);
    void setnonblocking();
    int getFd();
};
