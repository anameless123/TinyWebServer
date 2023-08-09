#pragma once
#include <sys/socket.h>
#include "InetAddress.hpp"
class Socket
{
private:
    int sockfd = 0;
    // IP ver, default = IPv4;
    InetAddress addr;
    // Max Connection, default = 128;
    int MaxCon = 128;
    

public:
    Socket();
    Socket(int sockfd, InetAddress addr);
    Socket(InetAddress addr);
    ~Socket();
    void bind(InetAddress addr);
    void listen();
    void connect(InetAddress addr);
    int accept(InetAddress *addr);
    void setnonblocking();

    int getFd();
    void printIP();
    void printPort();
};
