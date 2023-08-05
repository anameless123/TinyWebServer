#include "Socket.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include "../utils.hpp"

Socket::Socket()
{
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
}
Socket::Socket(__socket_type tran, uint16_t inet)
{
    errif(sockfd = ::socket(IP, tran, 0) == -1, "Failed to initialize socket!");
}
Socket::~Socket()
{
    close(sockfd);
}
void Socket::bind(InetAddress addr)
{
    if (addr.getv() == IP)
        errif(::bind(sockfd, addr.generilize(), addr.size()) == -1, "Failed to bind!");
    else
    {
        std::cout << "address ipVersion != socket ipVersion" << std::endl;
        exit(1);
    }
}

void Socket::connect(InetAddress addr)
{
    if (addr.getv() == IP)
        errif(::connect(sockfd, addr.generilize(), addr.size()) == -1, "Failed to connect!");
    else
    {
        std::cout << "address ipVersion != socket ipVersion" << std::endl;
        exit(1);
    }
}

void Socket::listen()
{
    errif(::listen(sockfd, MaxCon) == -1, "Failed to listen!");
}

int Socket::accept(InetAddress &clntaddr)
{
    socklen_t size = clntaddr.size();
    int res = ::accept(sockfd, clntaddr.generilize(), &size);
    if (size != clntaddr.size())
    {
        std::cout << "Failed to accept, address is cutdown" << std::endl;
        return -1;
    }
}
int Socket::getfd()
{
    return sockfd;
}